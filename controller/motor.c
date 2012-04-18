/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file 	/quadrofly/controller/motor.c/motor.c
 * @brief 	Motor control
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 19, 2012
 */

#include "main.h"
#include "global_def.h"
#include "i2cmaster.h"

#include <inttypes.h>

/**
 * Set the speed of the motors
 *
 * @param a Speed of motor A
 * @param b Speed of motor B
 * @param c Speed of motor C
 * @param d Speed of motor D
 * @return If the setting was succesful
 */
uint8_t motor_set(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    if (i2c_start(I2C_ADDR_MOTORCONTROL - 1 + I2C_WRITE)) {
        i2c_write(0x00);
        i2c_write(a);
        i2c_write(b);
        i2c_write(c);
        i2c_write(d);
        i2c_stop();
        return 1;
    } else {
        return 0;
    }

}
