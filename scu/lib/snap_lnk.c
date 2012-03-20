/*
 * Copyright (c) 2003 Jan Kl�tzke
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
 * @file 	receiver/lib/snap_lnk.c
 * @brief 	Link layer for uart interface.
 * @details Library from http://www.avrfreaks.net/index.php?module=Freaks%20Academy&func=viewItem&item_id=156&item_type=project More info about S.N.A.P. can be found on http://www.hth.com/snap/
 * @author  Jan Klötzke (jk177883@inf.tu-dresden.de)
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 11, 2012
 */
#include "main.h"
#include "snap_def.h"
#include "snap_lnk.h"
#include "rfm12.h"
#include "uart.h"
#include "log.h"
#include "i2cmaster.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * function from network layer handling received bytes
 */
#ifdef RFM12B_AVAILABLE
extern void snap_lnk_recv(uint8_t value, uint8_t err);

ISR(INT0_vect) {
	/* Read 1 byte snap packet:
	 *
	 * SYNC Synchronization byte
	 * HDB2 Header Definition Byte 2
	 * HDB1 Header Definition Byte 1
	 * DAB1 Destination Address Byte
	 * SAB1 Source Address Byte
	 * DB1  Data Byte 1
	 * CRC2 High byte of CRC-16
	 * CRC1 Low byte of CRC-16
	 */
	PORTC |= (1 << PC2);  // enable LED 2

	uint16_t rx = rfm12_rx();
	rfm12_write(0xCA80); // reset FIFO
	rfm12_write(0xCA83);
	//snap_lnk_recv(rx & 0xff00, 0); // lo byte
	if (!(i2c_start(RECEIVER_I2C_ADDR + I2C_WRITE))) //slave ready to write?
	{
		i2c_write(0x00); // set buffer start address
		i2c_write(rx & 0xff00);
		i2c_write(rx & 0x00ff);
		i2c_stop();
	}
	PORTC &= ~(1 << PC2);  // enable LED 2

}

#endif /* RFM12B_AVAILABLE */

/**
 * Snap link layer initialization
 */
void snap_lnk_init(void) {
#ifdef RFM12B_AVAILABLE
	/*
	 * Initialize RFM12B
	 */
	rfm12_init(); // Initialize module
	rfm12_setfreq(RF12FREQ(433.92));// set frequency to 433,92MHz
	rfm12_setbandwidth(4, 1, 4);// 200kHz bandwith, -6dB, DRSSI threshold: -79dBm
	rfm12_setbaud(19200);// 19200 BAUD
	rfm12_setpower(0, 6);// 1mW power, 120kHz frequency shift

	rfm12_rx_start();

	/*
	 * Initialize interrupt
	 */
	DDRD |= ~(1 << NIRQ); // set NIRQ to input
    EICRA |= (1 << ISC01); // The falling edge of INT0 generates an interrupt request
    EIMSK |= (1 << INT0); // enable INT0 interrupt
#endif /* RFM12B_AVAILABLE */
}

/**
 * Called to start sending of a snap packet
 */
void snap_lnk_send_start(void) {
#ifdef RFM12B_AVAILABLE
	rfm12_rx_done();
    EIMSK &= ~(1 << INT0); // disable INT0 interrupt
	rfm12_tx_start();
#endif /* RFM12B_AVAILABLE */
}

/**
 * Called to send a byte of the snap protocol
 *
 * @param value The byte to send
 */
void snap_lnk_send(uint8_t value) {
#ifdef RFM12B_AVAILABLE
	rfm12_tx(value);
#endif /* RFM12B_AVAILABLE */
#ifdef LOG_AVAILABLE
	log_i(value);
#endif /* LOG AVAILABLE */
}

/**
 * Called when the snap protocol finished sending
 */
void snap_lnk_send_done(void) {
#ifdef RFM12B_AVAILABLE
	rfm12_tx_done();
    EIMSK |= (1 << INT0); // enable INT0 interrupt
	rfm12_rx_start();
#endif /* RFM12B_AVAILABLE */
#ifdef LOG_AVAILABLE
	log_s("\n");
#endif /* LOG AVAILABLE */
}
