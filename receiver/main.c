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

#include "snap.h"

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

#ifdef SNAP_AVAILABLE
prog_char e_test[] = { 0xA0, 0x00, 0x01, 0x20, 0x37, 0x32, 0x78, 0x49 }; /*!< Some test value */

/**
 * Callback method that is called, if a snap packet is received
 *
 * @param status The status of the snap packet
 * @param *buf The buffer of the snap packet
 */
void snap_receive(uint8_t status, uint8_t *buf) {
	// do whatever you want, maybe...
	if (snap_fail(status))
		return;
	if (!snap_is_for_me())
		return;
}
#endif

/**
 * The main function.
 */
int main(void) {

	DDRC = 0xff; // set port c to output (there are our testing leds)

	/* Initialization */
	init_qfly();
	log_s("initialization ... ok\n");

	/*
	 * Initialize SNAP
	 */
#ifdef SNAP_AVAILABLE
	snap_init(&snap_receive);
#endif /* SNAP_AVAILABLE */

	/* Our loop */
	while (1) {

		/* Wait 500ms */
		_delay_ms(500);
		PORTC |= (1 << PC5);  // enable LED 1

		/* Wait 500ms */
		_delay_ms(500);
		PORTC &= ~(1 << PC5); // disable LED 1

#ifdef SNAP_AVAILABLE
		snap_send(e_test, SNAP_SEND_EDM_CRC_8 | SNAP_SEND_LEN_8 | SNAP_SEND_MEM_FLASH, 0x12);
#endif /* SNAP_AVAILABLE */

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
