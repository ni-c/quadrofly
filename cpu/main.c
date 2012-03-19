/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	cpu/main.c
 * @brief 	Main program
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "init.h"
#include "uart.h"
#include "log.h"
#include "i2cslave.h"

#include <avr/io.h>
#include <util/delay.h>

/**
 * The main function.
 */
int main(void) {

	/* Initialization */
	init_qfly();
	log_s("initialization... ok");

	/* Our loop */
	while (1) {

#ifdef I2C_SLAVE_AVAILABLE
		if (i2c_rx_ready()) {
#ifdef UART_AVAILABLE
			for (int i = 0; i < I2C_BUFFER_SIZE; ++i) {
				uart_tx_i(i2c_rx_buffer[i]);
			}
			uart_tx("\n");
#endif /* UART AVAILABLE */
		}
#endif /* I2C_SLAVE_AVAILABLE */

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
