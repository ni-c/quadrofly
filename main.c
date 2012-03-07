/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	main.c
 * @brief 	Main program
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "init.h"
#include "uart.h"
#include "debug.h"

#include <avr/io.h>
#include <util/delay.h>

/**
 * The main function.
 */
int main(int argc, char * argv[]) {

	/* Initialization */
	init_qfly(argc, argv);

	/* Our loop */
	while (1) {

		/* Write "Hello world!\n" to UART */
		uart_puts("Hello world!\n");

		/* Write "Hello debug!\n" to debug */
		debug_puts("Hello debug!\n");

		/* Wait a second */
		_delay_ms(1000);
	}

	/* Finally. (Never ever) */
	return 0;
}
