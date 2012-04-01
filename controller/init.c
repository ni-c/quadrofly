/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	cpu/init.c
 * @brief 	Global initialization file.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "global_def.h"
#include "uart.h"
#include "i2cslave.h"

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

#ifdef I2C_SLAVE_AVAILABLE
	i2c_slave_init(RECEIVER_I2C_ADDR);
#endif /* I2C_SLAVE_AVAILABLE */

	sei();

}
