/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	scu/init.c
 * @brief 	Global initialization file.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "i2cmaster.h"

#include <avr/interrupt.h>

/**
 * Quadrofly initialization
 */
void init_qfly(void) {

#ifdef I2C_AVAILABLE
	i2c_init();
#endif

	sei();
}
