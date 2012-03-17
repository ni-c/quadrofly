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
#include "log.h"

#include <avr/io.h>
#include <util/delay.h>

/**
 * The main function.
 */
int main(void) {

	/* Initialization */
	init_qfly();
	log_puts("initialization... ok\n");

	/* Our loop */
	while (1) {

		/* Wait a second */
		_delay_ms(1000);
	}

	/* Finally. (Never ever) */
	return 0;
}
