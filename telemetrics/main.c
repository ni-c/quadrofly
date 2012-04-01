/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	receiver/main.c
 * @brief 	Main program
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "global_def.h"
#include "init.h"
#include "uart.h"
#include "log.h"
#include "rfm12.h"

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>


/**
 * Received a rf12b packet
 *
 * @param value The packet data
 */
void rfm12_receive(uint8_t value) {

#ifdef UART_AVAILABLE
	uart_tx_i(value);
#endif /* UART_AVAILABLE */
}

/**
 * The main function.
 */
int main(void) {

	DDRA = 0xff; // set port c to output (there are our testing leds)

	/* Initialization */
	init_qfly();
	log_s("receiver initialization ... ok\n");

	/* Our loop */
	while (1) {

		/* Wait 500ms */
		_delay_ms(500);
		PORTA |= (1 << PA0);  // enable LED 1
		PORTA &= ~(1 << PA1); // disable LED 1

		/* Wait 500ms */
		_delay_ms(500);
		PORTA &= ~(1 << PA0); // disable LED 1
		PORTA &= ~(1 << PA1); // disable LED 1

#ifdef UART_AVAILABLE
		if (uart_rx_ready()) {
			uart_tx("Echo: ");
			uart_tx(uart_rx());
			uart_tx("\n");
		}
#endif /* UART_AVAILABLE */
	}

	/* Finally. (Never ever) */
	return 0;
}
