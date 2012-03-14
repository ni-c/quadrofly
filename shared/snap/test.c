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
 *
 *
 * Small program to show how to use the snap functions. Does nothing really usefull.
 */

#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include "snap.h"

prog_char e_test[] = { 0x00, 0x12, 0x01, 0x20, 0x37, 0x32, 0x78, 0x49 };

INTERRUPT(SIG_OVERFLOW0) {
	TCNT0 = 0xff - 93;
	snap_check_timeout();
}

// received a packet
void receive(uint8_t status, uint8_t *buf) {
	// do whatever you want, maybe...
	if (snap_fail(status))
		return;
	if (!snap_is_for_me())
		return;

	//...
}

int main(void) {
	// init subsystems
	sei();
	snap_init(&receive);
	// ...

	// configure timer
	TCNT0 = 0xff - 93;
	TCCR0A = _BV(CS01) | _BV(CS00);
	sbi(TIMSK0, TOIE0);

	while (1) {
		// main loop
		snap_send(e_test,
				SNAP_SEND_EDM_CRC_16 | SNAP_SEND_LEN_8 | SNAP_SEND_MEM_FLASH,
				0x1234);
	}

	return 0;
}
