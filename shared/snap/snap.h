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
 * @file 	snap.h
 * @brief 	A nearly full implementation of the S.N.A.P protocol from HTH for avr-gcc.
 * @details Library from http://www.avrfreaks.net/index.php?module=Freaks%20Academy&func=viewItem&item_id=156&item_type=project More info about S.N.A.P. can be found on http://www.hth.com/snap/
 * @author  Jan Klötzke (jk177883@inf.tu-dresden.de)
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 11, 2012
 */

#ifndef __SNAP_H
#define __SNAP_H

#include <inttypes.h>
#include "snap_def.h"

#if SNAP_MAX_ADDRESS_BYTES == 3
#	define SNAP_ADDRESS_T	uint32_t
#elif SNAP_MAX_ADDRESS_BYTES == 2
#	define SNAP_ADDRESS_T	uint16_t
#elif SNAP_MAX_ADDRESS_BYTES == 1
#	define SNAP_ADDRESS_T	uint8_t
#elif SNAP_MAX_ADDRESS_BYTES == 0
#	undef SNAP_ADDRESS_T
#else
#	error "Wrong number of address bytes! Check SNAP_MAX_ADDRESS_BYTES."
#endif
#if (SNAP_MAX_PROTOCOL_BYTES > 3) || (SNAP_MAX_PROTOCOL_BYTES < 0)
#	error "Wrong number of protocol specific bytes! Check SNAP_MAX_PROTOCOL_BYXTES."
#endif

#ifdef SNAP_ADDRESS_T
#	define SNAP_ADDR_BROADCAST	((ADDRESS_T)0)
#else
#	undef SNAP_ADDR_BROADCAST
#endif

#if SNAP_MAX_PACKET_LENGTH > 255
#	define SNAP_LENGTH_T	uint8_t
#else
#	define SNAP_LENGTH_T	uint16_t
#endif

/**
 * callback function for packet receive
 */
typedef void SNAP_RECV_FUNC_T(uint8_t status, uint8_t *buf);

#define SNAP_RECV_OK		0	/* all fine                                      */
#define SNAP_RECV_CORRECTED	1	/* edm found error(s), but corrected through fec */
#define SNAP_RECV_ERR_EDM	2	/* edm found error(s), not corrected             */
#define SNAP_RECV_ERR_INVAL	3	/* received invalid packet, handle with caution  */
#define SNAP_RECV_ERR_ABORT	4	/* receive aborted due to timeout or overrun     */

#define snap_success(x) (x <= SNAP_RECV_CORRECTED)
#define snap_fail(x) (x >= SNAP_RECV_ERR_EDM)

/*
 * global functions
 */
void snap_init(SNAP_RECV_FUNC_T *callback);
void snap_check_timeout(void);
#ifdef SNAP_ADDRESS_T
#	ifdef SNAP_STATIC_ADDR
		void snap_send(uint8_t *buf, uint16_t flags, SNAP_ADDRESS_T to);
#	else
		void snap_send(uint8_t *buf, uint16_t flags, SNAP_ADDRESS_T from, SNAP_ADDRESS_T to);
#	endif
#else
	void snap_send(uint8_t *buf, uint16_t flags);
#endif

#ifdef SNAP_ADDRESS_T
SNAP_ADDRESS_T snap_get_destination_addr(void);
SNAP_ADDRESS_T snap_get_source_addr(void);
#ifdef SNAP_STATIC_ADDR
uint8_t snap_is_for_me(void);
#endif
#endif
uint8_t snap_get_ack(void);
SNAP_LENGTH_T snap_get_length(void);

/*
 * snap_get_ack flags
 */
#define SNAP_REQU_NO_ACK	0x00
#define SNAP_REQU_ACK		0x01
#define SNAP_REPLY_ACK		0x02
#define SNAP_REPLY_NACK		0x03

/**
 * net_send flags
 */
#define SNAP_SEND_REQU_NO_ACK		0x0000
#define SNAP_SEND_REQU_ACK		0x0100
#define SNAP_SEND_REPLY_ACK		0x0200
#define SNAP_SEND_REPLY_NACK		0x0300

#define SNAP_SEND_CMD			0x0080

#define SNAP_SEND_EDM_NONE		0x0000
#define SNAP_SEND_EDM_CHECKSUM_8	0x0020
#define SNAP_SEND_EDM_CRC_8		0x0030
#define SNAP_SEND_EDM_CRC_16		0x0040
#define SNAP_SEND_EDM_CRC_32		0x0050
//#define SNAP_SEND_EDM_FEC		0x0060

#define SNAP_SEND_LEN_0			0x0000
#define SNAP_SEND_LEN_1			0x0001
#define SNAP_SEND_LEN_2			0x0002
#define SNAP_SEND_LEN_3			0x0003
#define SNAP_SEND_LEN_4			0x0004
#define SNAP_SEND_LEN_5			0x0005
#define SNAP_SEND_LEN_6			0x0006
#define SNAP_SEND_LEN_7			0x0007
#define SNAP_SEND_LEN_8			0x0008
#define SNAP_SEND_LEN_16		0x0009
#define SNAP_SEND_LEN_32		0x000a
#define SNAP_SEND_LEN_64		0x000b
#define SNAP_SEND_LEN_128		0x000c
#define SNAP_SEND_LEN_256		0x000d
#define SNAP_SEND_LEN_512		0x000e

#define SNAP_SEND_MEM_RAM		0x0000
#define SNAP_SEND_MEM_FLASH		0x0400

#endif
