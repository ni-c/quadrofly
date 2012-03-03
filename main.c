/**
 * main.c
 *
 *  The main routine.
 *
 *  @date Mar 3, 2012
 *  @author Willi Thiel
 */

#include <avr/io.h>

/**
 * The main function.
 */
int main(void) {

	/* Set port PA0 to output */
	DDRA = (1 << DDA0);

	/* Pull port PA0 up */
	PORTA = (1 << PA0);

	while (1) {
		/* Our loop */
	}

	/* Finally. (Never ever) */
	return 0;
}
