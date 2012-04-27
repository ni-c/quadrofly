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
#include "i2cslave.h"
#include "motor.h"
#include "rx.h"

#include <avr/interrupt.h>

/**
 * Quadrofly initialization
 */
void init_qfly(void) {

#ifdef I2C_SLAVE_AVAILABLE
    /*
     * Initialize I2C slave
     */
    i2c_slave_init(I2C_ADDR_MOTORCONTROL);
    i2c_buffer[0] = 0;
    i2c_buffer[1] = 0;
    i2c_buffer[2] = 0;
    i2c_buffer[3] = 0;
#endif /* I2C_SLAVE_AVAILABLE */

#ifdef MOTOR_AVAILABLE
    /*
     * Initialize motors
     */
    motor_init();
#endif /* MOTOR_AVAILABLE */

#ifdef RX_AVAILABLE
    /*
     * Initialize motors
     */
    rx_init();
#endif /* RX_AVAILABLE */

    /*
     * Enable global interupts
     */
    sei();
}
