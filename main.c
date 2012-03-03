/**
 * main.c
 *
 *  The main routine.
 *
 *  Created on: Mar 3, 2012
 *      Author: Willi Thiel
 */

#include <avr/io.h>

/**
 * The main function.
 */
int main(void) {

	DDRA = 0xFF;

	PORTB = (1<<PB1) | (1<<PB0);

	while (1) {
		/* Our loop */
	}

	/* Finally. (Never ever) */
	return 0;
}
