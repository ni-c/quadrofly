/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	scu/main.c
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

	/* Our loop */
	while (1) {

#ifdef UART_AVAILABLE
		if (uart_rx_ready()) {
			uart_tx("Echo: ");
			uart_tx(uart_rx);
		}
#endif /* UART_AVAILABLE */
	}

	/* Finally. (Never ever) */
	return 0;
}
