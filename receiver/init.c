/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	receiver/init.c
 * @brief 	Global initialization file.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "uart.h"

#include <avr/interrupt.h>

/**
 * Quadrofly initialization
 */
void init_qfly(void) {

	/*
	 * Initialize UART
	 */
#ifdef UART_AVAILABLE
	uart_init();
#endif

	sei();

}
