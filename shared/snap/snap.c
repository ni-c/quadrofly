/*
 * Copyright (c) 2003 Jan Klötzke
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	snap.c
 * @brief 	A nearly full implementation of the S.N.A.P protocol from HTH for avr-gcc.
 * @details Library from http://www.avrfreaks.net/index.php?module=Freaks%20Academy&func=viewItem&item_id=156&item_type=project More info about S.N.A.P. can be found on http://www.hth.com/snap/
 * @author  Jan Klötzke (jk177883@inf.tu-dresden.de)
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 11, 2012
 */

#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include "snap_def.h"
#include "snap_lnk.h"
#include "snap.h"

// determine some types
#if defined (SNAP_EDM_CRC_32)
#	define SNAP_MAX_EDM_BYTES	4
#elif defined (SNAP_EDM_CRC_16)
#	define SNAP_MAX_EDM_BYTES	2
#elif defined (SNAP_EDM_CRC_8)
#	define SNAP_MAX_EDM_BYTES	1
#elif defined (SNAP_EDM_CHECKSUM_8)
#	define SNAP_MAX_EDM_BYTES	1
#else
#	define SNAP_MAX_EDM_BYTES	0
#endif
#define SNAP_MAX_TOTAL_LENGTH	(2+SNAP_MAX_PACKET_LENGTH+(SNAP_MAX_ADDRESS_BYTES*2)+\
				SNAP_MAX_PROTOCOL_BYTES+SNAP_MAX_EDM_BYTES)
#if SNAP_MAX_TOTAL_LENGTH > 256
#	define LENGTH_T	uint16_t
#else
#	define LENGTH_T	uint8_t
#endif

#if defined(SNAP_EDM_CHECKSUM_8) || defined(SNAP_EDM_CRC_8) ||   \
	defined(SNAP_EDM_CRC_16) || defined(SNAP_EDM_CRC_32) ||  \
	defined(SNAP_EDM_FEC)
#define SNAP_EDM_ANY
#endif

typedef union {
#ifdef SNAP_EDM_CHECKSUM_8
	uint8_t checksum;
#endif
#ifdef SNAP_EDM_CRC_8
	uint8_t crc08;
#endif
#ifdef SNAP_EDM_CRC_16
	uint16_t crc16;
#endif
#ifdef SNAP_EDM_CRC_32
	u32 crc32;
#endif
	uint8_t crc_raw[SNAP_MAX_EDM_BYTES];
} edm_stat_u;

// current receive status
typedef enum {
	STAT_IDLE, // listen for packet
	STAT_RECV_HEADER, // receive header
	STAT_RECV_BODY, // receive packet body
	STAT_RECV_RESYNC, // lost sync
	STAT_RECV_EXAMINE, // got whole packet
	STAT_SEND // sending packet
} recv_stat_e;

// crc polynomials
#define POLY_CRC08	0x18
#define	POLY_CRC16	0x1021
#define POLY_CRC32	0xedb88320	/* original 0x04c11db7, but reflected for algo */

// address byte constants
#define SAB_0		0x0000
#define SAB_1		0x1000
#define SAB_2		0x2000
#define SAB_3		0x3000
#define DAB_0		0x0000
#define DAB_1		0x4000
#define DAB_2		0x8000
#define DAB_3		0xc000

// misc
#define SYNC		0x54
#define HDB2		(recv_buffer[0])
#define HDB1		(recv_buffer[1])
#define EDM_MASK	0x70

// local variables
static volatile recv_stat_e status;
static volatile LENGTH_T bytes_to_go, bytes_to_crc, recv_pos;
static uint8_t recv_buffer[SNAP_MAX_TOTAL_LENGTH];
static SNAP_RECV_FUNC_T *recv_func;
static volatile uint8_t timeout = 0;
#ifdef SNAP_EDM_ANY
static edm_stat_u edm_stat;
#endif

// packet length tables, stored in falsh
#if SNAP_MAX_PACKET_LENGTH >= 256
static prog_int dat_length_table[] = {
#else
static prog_uchar dat_length_table[] = {
#endif
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x10, 0x20, 0x40,
		0x80,
#if SNAP_MAX_PACKET_LENGTH < 256
		0xf0, 0xf0
#else
		0x100, 0x200
#endif
		};
static prog_char edm_length_table[] = { 0x00, 0x00, 0x01, 0x01, 0x02, 0x04,
		0x00, 0x00 };

/*
 * receive timeout functions
 */

static inline void start_recv_timeout(void) {
	timeout = SNAP_NET_TIMEOUT;
}

static inline void stop_recv_timeout(void) {
	timeout = 0;
}

#define reset_recv_timeout() start_recv_timeout()

void snap_check_timeout(void) {
	// timer running?
	if (timeout > 0) {
		timeout--;
		// timed out?
		if (timeout == 0) {
			status = STAT_IDLE;
			recv_func(SNAP_RECV_ERR_ABORT, (uint8_t*) 0);
		}
	}
}

/*
 * EDM functions.
 */
#ifdef SNAP_EDM_CHECKSUM_8
static void inline edm_checksum_8_init(void)
{
	edm_stat.checksum = 0;
}
static void inline edm_checksum_8_add(uint8_t value)
{
	edm_stat.checksum += value;
}
static uint8_t inline edm_checksum_8_get(void)
{
	return edm_stat.checksum;
}
#endif

#ifdef SNAP_EDM_CRC_8
/***
 * CRC8 routines taken from Colin O'Flynn, Copyright (c) 2002.
 * The original code can be foud at www.avrfreaks.net as 
 * project 104: "Error Detection Method routines".
 */
static void inline edm_crc_8_init(void)
{
	edm_stat.crc08 = 0x00;
}
static void edm_crc_8_add(uint8_t value)
{
	uint8_t i, feedback_bit;

	for (i=0; i<8; i++) {
		feedback_bit = (edm_stat.crc08 ^ value) & 0x01;
		if (feedback_bit != 0) {
			edm_stat.crc08 ^= POLY_CRC08;
		}
		edm_stat.crc08 = (edm_stat.crc08 >> 1) & 0x7F;
		if (feedback_bit != 0) {
			edm_stat.crc08 |= 0x80;
		}

		value >>= 1;
	};
}
static uint8_t inline edm_crc_8_get(void)
{
	return edm_stat.crc08;
}
#endif

#ifdef SNAP_EDM_CRC_16
static void inline edm_crc_16_init(void) {
	edm_stat.crc16 = 0x0000;
}
static void edm_crc_16_add(uint8_t value) {
	uint8_t i;

	edm_stat.crc16 ^= ((uint16_t) value << 8);
	for (i = 0; i < 8; i++) {
		if (edm_stat.crc16 & 0x8000)
			edm_stat.crc16 = (edm_stat.crc16 << 1) ^ POLY_CRC16;
		else
			edm_stat.crc16 <<= 1;
	};
}
#endif

#ifdef SNAP_EDM_CRC_32
static void inline edm_crc_32_init(void)
{
	edm_stat.crc32 = 0xffffffff;
}
static void edm_crc_32_add(uint8_t value)
{
	uint8_t i;

	edm_stat.crc32 ^= value;
	for (i=0; i<8; i++) {
		if (edm_stat.crc32 & 0x01)
		edm_stat.crc32 = (edm_stat.crc32 >> 1) ^ POLY_CRC32;
		else
		edm_stat.crc32 >>= 1;
	};
}
static u32 inline edm_crc_32_get(void)
{
	return ~edm_stat.crc32;
}
#endif

/**
 * Init error detection, detect type from header.
 * Returns TRUE if successful, otherwise FALSE.
 */
static int8_t edm_init(uint8_t hdb1) {
	switch (hdb1 & EDM_MASK) {
	case SNAP_SEND_EDM_NONE:
		return 1;
#ifdef SNAP_EDM_CHECKSUM_8
		case SNAP_SEND_EDM_CHECKSUM_8:
		edm_checksum_8_init();
		return 1;
#endif
#ifdef SNAP_EDM_CRC_8
		case SNAP_SEND_EDM_CRC_8:
		edm_crc_8_init();
		return 1;
#endif
#ifdef SNAP_EDM_CRC_16
	case SNAP_SEND_EDM_CRC_16:
		edm_crc_16_init();
		return 1;
#endif
#ifdef SNAP_EDM_CRC_32
		case SNAP_SEND_EDM_CRC_32:
		edm_crc_32_init();
		return 1;
#endif
	default:
		return 0;
	};
	return 0;
}

/**
 * Add a byte to the active checksum.
 */
static void edm_process(uint8_t hdb1, uint8_t value) {
	switch (hdb1 & EDM_MASK) {
#ifdef SNAP_EDM_CHECKSUM_8
	case SNAP_SEND_EDM_CHECKSUM_8:
	edm_checksum_8_add(value);
	break;
#endif
#ifdef SNAP_EDM_CRC_8
	case SNAP_SEND_EDM_CRC_8:
	edm_crc_8_add(value);
	break;
#endif
#ifdef SNAP_EDM_CRC_16
	case SNAP_SEND_EDM_CRC_16:
		edm_crc_16_add(value);
		break;
#endif
#ifdef SNAP_EDM_CRC_32
		case SNAP_SEND_EDM_CRC_32:
		edm_crc_32_add(value);
		break;
#endif
	};
}

/**
 * Received a byte...
 * Called from the link layer.
 */
void snap_lnk_recv(uint8_t value, uint8_t err) {
	uint8_t crc_stat, offset;

	switch (status) {
	case STAT_IDLE:
		// In case of an error simply do nothing since we have
		// no further information. Don't know something better.
		if (err != 0)
			return;
		// look for sync
		if (value != SYNC)
			return;
		status = STAT_RECV_HEADER;
		recv_pos = 0;
		start_recv_timeout();
		break;

	case STAT_RECV_HEADER:
		reset_recv_timeout();
		if (err != 0) {
			// can't estimate packet length so simply look
			// for new sync-byte
			status = STAT_IDLE;
			stop_recv_timeout();
			return;
		}
		recv_buffer[recv_pos++] = value;
		if (recv_pos < 2)
			break;

		// got header bytes, now do the rest
		bytes_to_crc = __LPM(dat_length_table+(HDB1 & 0x0f)) + // length
				((HDB2 >> 2) & 0x03) + // PFB
				((HDB2 >> 4) & 0x03) + // SAB
				((HDB2 >> 6) & 0x03); // DAB
		bytes_to_go = bytes_to_crc
				+ __LPM(edm_length_table+((HDB1 >> 4) & 0x07)); // EDM
#ifdef SNAP_SECURE
		if (!edm_init(HDB1)) {
			status = STAT_RECV_RESYNC;
			recv_func(SNAP_RECV_ERR_INVAL, (uint8_t*) 0);
			return;
		}
		;
#elif SNAP_EDM_ANY
		edm_init(HDB1);
#endif
#ifdef SNAP_EDM_ANY
		edm_process(HDB1, HDB2);
		edm_process(HDB1, HDB1);
#endif
		if (bytes_to_go == 0) {
			status = STAT_RECV_EXAMINE;
			stop_recv_timeout();
			goto process_data;
		} else {
			status = STAT_RECV_BODY;
		}
		break;

	case STAT_RECV_BODY:
		reset_recv_timeout();
#ifdef SNAP_SECURE
		if (recv_pos >= SNAP_MAX_TOTAL_LENGTH) {
			// bad boys...
			status = STAT_IDLE;
			stop_recv_timeout();
			recv_func(SNAP_RECV_ERR_INVAL, (uint8_t*) 0);
			return;
		}
#endif
		// do not test for framing errors, may have been some 
		// noise (hopefully :-))
		if ((err & ~SNAP_LNK_ERR_FRAMING) != 0) {
			// lost at least one byte
			status = STAT_RECV_RESYNC;
			if (bytes_to_go > 0)
				bytes_to_go--;
			if (bytes_to_go > 0)
				bytes_to_go--;
			recv_func(SNAP_RECV_ERR_ABORT, (uint8_t*) 0);
			return;
		}
		recv_buffer[recv_pos++] = value;
		if (bytes_to_crc > 0) {
			bytes_to_crc--;
			edm_process(HDB1, value);
		}

		// some bytes remaining?
		bytes_to_go--;
		if (bytes_to_go > 0)
			return;
		// got it, examine data...
		// starting from here we have to be quick since any new
		// packet would be lost!
		stop_recv_timeout();
		status = STAT_RECV_EXAMINE;
		goto process_data;
		break;

	case STAT_RECV_RESYNC:
		reset_recv_timeout();
		if (bytes_to_go > 0)
			bytes_to_go--;
		if ((bytes_to_go > 0) && (err != 0))
			bytes_to_go--;
		if (bytes_to_go == 0) {
			stop_recv_timeout();
			status = STAT_IDLE;
		}
		break;

	default:
		// should not happen at all...
		// may trigger some kind of error????
		break;
	}
	return;

	process_data:
	sei();

	// check checksum
	crc_stat = SNAP_RECV_ERR_EDM;
	recv_pos--;
	switch (HDB1 & EDM_MASK) {
#ifdef SNAP_EDM_CRC_32
	case SNAP_SEND_EDM_CRC_32:
	if (edm_stat.crc_raw[3] != recv_buffer[recv_pos-3]) break;
	if (edm_stat.crc_raw[2] != recv_buffer[recv_pos-2]) break;
#endif
#if defined(SNAP_EDM_CRC_16) || defined (SNAP_EDM_CRC_32)
	case SNAP_SEND_EDM_CRC_16:
		if (edm_stat.crc_raw[1] != recv_buffer[recv_pos - 1])
			break;
		/* no break */
#endif
#if defined(SNAP_EDM_CRC_16) || defined(SNAP_EDM_CRC_32) || \
	    defined(SNAP_EDM_CRC_8) || defined(SNAP_EDM_CHECKSUM_8)
	case SNAP_SEND_EDM_CRC_8:
	case SNAP_SEND_EDM_CHECKSUM_8:
		if (edm_stat.crc_raw[0] != recv_buffer[recv_pos])
			break;
		/* no break */
#endif
	case SNAP_SEND_EDM_NONE:
		crc_stat = SNAP_RECV_OK;
		break;
	default:
		crc_stat = SNAP_RECV_ERR_INVAL;
		break;
	}

	// number of address bytes supported?
#ifdef SNAP_SECURE
	if (((HDB2 & 0x30) > (SNAP_MAX_ADDRESS_BYTES << 4))
			|| ((HDB2 & 0xc0) > (SNAP_MAX_ADDRESS_BYTES << 6)))
		crc_stat = SNAP_RECV_ERR_INVAL;
#endif

	// call handler
	offset = ((HDB2 >> 2) & 0x03) + // PFB
			((HDB2 >> 4) & 0x03) + // SAB
			((HDB2 >> 6) & 0x03); // DAB
	recv_func(crc_stat, recv_buffer + offset + 2);
	status = STAT_IDLE;
}

void snap_init(SNAP_RECV_FUNC_T *callback) {
	status = STAT_IDLE;
	recv_func = callback;
	snap_lnk_init(); // init link layer
}

static void send_with_edm(uint8_t hdb1, uint8_t value) {
	snap_lnk_send(value);
	edm_process(hdb1, value);
}

/**
 * Send a packet.
 */
#ifdef SNAP_ADDRESS_T
#	ifdef SNAP_STATIC_ADDR
void snap_send(uint8_t *buf, uint16_t flags, SNAP_ADDRESS_T to) {
#	else
	void snap_send(uint8_t *buf, uint16_t flags, SNAP_ADDRESS_T from, SNAP_ADDRESS_T to) {
#	endif
#else
	void snap_send(uint8_t *buf, uint16_t flags) {
#endif

	LENGTH_T length, j;
	uint8_t i, use_flash;

	// which memory to use
	use_flash = ((flags & SNAP_SEND_MEM_FLASH) != 0);
	flags &= 0xf3ff;

	// let's start...
	do {
		cli();
		if (status == STAT_IDLE)
			status = STAT_SEND;
		sei();
	} while (status != STAT_SEND);
	snap_lnk_send_start();
	edm_init(flags);
	snap_lnk_send(SYNC);

	// send header and address bytes
#ifndef SNAP_ADDRESS_T
	// no address bytes at all
	send_with_edm(flags, ((uint8_t*)&flags)[1]);
	send_with_edm(flags, ((uint8_t*)&flags)[0]);
#else
	// determine address bytes, update header
	// sab
#ifdef SNAP_STATIC_ADDR
#if SNAP_STATIC_ADDR <= 0xff
	flags |= SAB_1;
#elif SNAP_STATIC_ADDR <= 0xffff
	flags |= SAB_2;
#else
	flags |= SAB_3;
#endif
#else
	uint8_t sab = 0;

#if SNAP_MAX_ADDRESS_BYTES > 1
	if (from <= 0xff) {
#endif
		sab = 1;
		flags |= SAB_1;
#if SNAP_MAX_ADDRESS_BYTES > 1
	}
	else if (from <= 0xffff) {
		sab = 2;
		flags |= SAB_2;
	}
#endif
#if SNAP_MAX_ADDRESS_BYTES > 2
	else {
		sab = 3;
		flags |= SAB_3;
	}
#endif
#endif

	// dab
	uint8_t dab = 0;

#if SNAP_MAX_ADDRESS_BYTES > 1
	if (to <= 0xff) {
#endif
		dab = 1;
		flags |= DAB_1;
#if SNAP_MAX_ADDRESS_BYTES > 1
	} else if (to <= 0xffff) {
		dab = 2;
		flags |= DAB_2;
	}
#endif
#if SNAP_MAX_ADDRESS_BYTES > 2
	else {
		dab = 3;
		flags |= DAB_3;
	}
#endif

	// send header
	send_with_edm(flags, ((uint8_t*) &flags)[1]);
	send_with_edm(flags, ((uint8_t*) &flags)[0]);

	// send address bytes
	for (i = dab; i > 0; i--)
		send_with_edm(flags, ((uint8_t*) &to)[i - 1]);
#ifdef SNAP_STATIC_ADDR
#if SNAP_STATIC_ADDR > 0xffff
	send_with_edm(flags, (uint8_t)(SNAP_STATIC_ADDR>>16));
#endif
#if SNAP_STATIC_ADDR > 0xff
	send_with_edm(flags, (uint8_t)(SNAP_STATIC_ADDR>>8));
#endif
	send_with_edm(flags, (uint8_t) SNAP_STATIC_ADDR);
#else
	for (i=sab; i>0; i--) send_with_edm(flags, ((uint8_t*)&from)[i-1]);
#endif
#endif

	// send data
	length = __LPM(dat_length_table+(flags & 0x0f));
	if (use_flash) {
		for (j = 0; j < length; j++)
			send_with_edm(flags, __LPM(buf+j));
	} else {
		for (j = 0; j < length; j++)
			send_with_edm(flags, buf[j]);
	}

	// send crc
#ifdef SNAP_EDM_ANY
	length = __LPM(edm_length_table+((flags >> 4) & 0x07));
	for (i = length; i > 0; i--)
		snap_lnk_send(((uint8_t*) &edm_stat)[i - 1]);
#endif

	// done
	status = STAT_IDLE;
	snap_lnk_send_done();
}

#ifdef SNAP_ADDRESS_T

/**
 * Return destination address of last received packet.
 *
 * If zero is returned this may indicate a not supported packet or a broadcast.
 * Always have a look at the status passed to your callback function. If it is
 * SNAP_RECV_ERR_INVAL this function may not return a address but zero.
 */SNAP_ADDRESS_T snap_get_destination_addr(void) {
#ifdef SNAP_SECURE
	if ((HDB2 & 0xc0) > (SNAP_MAX_ADDRESS_BYTES << 6))
		return 0;
#endif

#if SNAP_MAX_ADDRESS_BYTES > 1
	SNAP_ADDRESS_T addr;
	uint8_t i, len, base;

	addr = 0;
	len = (HDB2 >> 6) & 0x03;
	base = 1 + len;
	for (i = 0; i < len; i++)
		((uint8_t*) &addr)[i] = recv_buffer[base - i];
	return addr;
#else
	return recv_buffer[2];
#endif
}

/**
 * Return source address of last received packet.
 *
 * Returning zero means the packet is not supported.
 */SNAP_ADDRESS_T snap_get_source_addr(void) {
#ifdef SNAP_SECURE
	if ((HDB2 & 0x30) > (SNAP_MAX_ADDRESS_BYTES << 4))
		return 0;
#endif

#if SNAP_MAX_ADDRESS_BYTES > 1
	SNAP_ADDRESS_T addr;
	uint8_t i, len, base;

	addr = 0;
	len = (HDB2 >> 4) & 0x03;
	base = ((HDB2 >> 6) & 0x03) + 1 + len;
	for (i = 0; i < len; i++)
		((uint8_t*) &addr)[i] = recv_buffer[base - i];
	return addr;
#else
	return recv_buffer[3];
#endif
}

#ifdef SNAP_STATIC_ADDR
/**
 * Returns a nonzero value if the last received packet was a broadcast or the
 * destination address matches SNAP_STATIC_ADDR.
 * Otherwise return zero.
 */
uint8_t snap_is_for_me(void) {
#if SNAP_MAX_ADDRESS_BYTES > 1
	SNAP_ADDRESS_T addr;
	uint8_t i, len, base;

	addr = SNAP_STATIC_ADDR;
	len = (HDB2 >> 6) & 0x03;
	base = 1 + len;

	for (i = 0; i < len; i++)
		if (((uint8_t*) &addr)[i] != recv_buffer[base - i])
			goto test_broadcast;
	return 1;

	test_broadcast: for (i = 0; i < len; i++)
		if (recv_buffer[base - i] != 0)
			return 0;
	return 1;
#else
	return ((recv_buffer[2] == SNAP_STATIC_ADDR) || (recv_buffer[2] == 0));
#endif
}
#endif

#endif

uint8_t snap_get_ack(void) {
	return (HDB2 & 0x03);
}

SNAP_LENGTH_T snap_get_length(void) {
	return __LPM(dat_length_table+(HDB1 & 0x0f));
}
