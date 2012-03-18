/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	receiver/main.c
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

	DDRC = 0xff; // set port c to output (there are our testing leds)

	/* Initialization */
	init_qfly();
	log_s("Initialization... ok\n");

	/* Our loop */
	while (1) {

		/* Wait a second */
		_delay_ms(500);
		PORTC |= (1 << PC5);  // enable LED 1

		uart_tx("Hello world!\n");

		/* Wait a second */
		_delay_ms(500);
		PORTC &= ~(1 << PC5); // disable LED 1

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
