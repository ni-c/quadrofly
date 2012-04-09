/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	motorcontrol/main.c
 * @brief 	Main program
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "global_def.h"
#include "init.h"
#include "motor.h"
#include "i2cslave.h"

#include <avr/delay.h>

/**
 * The main function.
 */
int main(void) {

    /* Initialization */
    init_qfly();

    DDRB = (1 << PB4);

    /* Our loop */
    while (1) {

#ifdef I2C_SLAVE_AVAILABLE
        if (i2c_rx_ready()) {
            motor_set(0, i2c_rx_buffer[0]);
            motor_set(1, i2c_rx_buffer[1]);
            motor_set(2, i2c_rx_buffer[2]);
            motor_set(3, i2c_rx_buffer[3]);
        }
#endif /* I2C_SLAVE_AVAILABLE */
    }

    /* Finally. (Never ever) */
    return 0;
}
