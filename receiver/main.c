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
}


/**
 * The main function.
 */
int main(void) {

	DDRC = 0xff; // set port c to output (there are our testing leds)

	/* Initialization */
	init_qfly();
	log_s("initialization ... ok\n");


	/* Our loop */
	while (1) {

		/* Wait 500ms */
		_delay_ms(500);
		PORTC |= (1 << PC5);  // enable LED 1

		/* Wait 500ms */
		_delay_ms(500);
		PORTC &= ~(1 << PC5); // disable LED 1

#ifdef RFM12B_AVAILABLE
		rfm12_send(0xee);
#endif

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
