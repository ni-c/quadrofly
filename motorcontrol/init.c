/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	motorcontrol/init.c
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

#ifdef UART_AVAILABLE
    /*
     * Initialize UART
     */
    uart_init();
#endif

#ifdef I2C_SLAVE_AVAILABLE
    /*
     * Initialize I2C slave
     */
    i2c_slave_init(RECEIVER_I2C_ADDR);
#endif /* I2C_SLAVE_AVAILABLE */

    /*
     * Enable global interupts
     */
    sei();
}
