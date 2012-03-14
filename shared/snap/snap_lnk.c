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
 * @file 	snap_lnk.c
 * @brief 	Link layer for uart interface.
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

// function from network layer handling received bytes
extern void snap_lnk_recv(uint8_t value, uint8_t err);

SIGNAL(SIG_UART_RECV) {
	uint8_t err, value;

	if ((UCSR0A & _BV(FE0)) != 0)
		err = SNAP_LNK_ERR_FRAMING;
	else
		err = 0;
	value = UDR0;
	if ((UCSR0A & _BV(DOR0)) != 0)
		err |= SNAP_LNK_ERR_OVERRUN;
	snap_lnk_recv(value, err);
}

void snap_lnk_init(void) {
	UBRR0 = SNAP_F_CLK / (SNAP_BAUD_RATE * 16l) - 1; // set baud rate
	sbi(SEND_PORT, SEND_PIN); // configure send pin
	sbi(SEND_DDR, SEND_PIN);
	UCSR0A |= _BV(RXCIE0) | _BV(RXEN0); // enable receiver+interrupt
}

void snap_lnk_send_start(void) {
	cbi(UCSR0A, RXEN0); // disable receiver
	sbi(UCSR0A, TXEN0); // enable transmitter
	cbi(SEND_PORT, SEND_PIN); // enable driver
	UCSR0A = _BV(TXC0); // clear txc flag
}

void snap_lnk_send(uint8_t value) {
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = value;
}

void snap_lnk_send_done(void) {
	loop_until_bit_is_set(UCSR0A, TXC0);
	// wait for end of transmission
	sbi(SEND_PORT, SEND_PIN); // disable driver
	cbi(UCSR0A, TXEN0); // disable transmitter
	sbi(UCSR0A, RXEN0); // enable receiver
}
