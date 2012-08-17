/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file 	controller/lib/motorcom.c
 * @brief 	Communication with the motorcontrol
 * @ingroup controller
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 19, 2012
 */

#include "main.h"

#ifdef MOTORCONTROL_AVAILABLE
#include "global_def.h"
#include "i2cmaster.h"
#include "motorcontrol.h"

#include <inttypes.h>
#endif /* MOTORCONTROL_AVAILABLE */

/**
 * Writes value to the ENABLE register of the motorcontrol unit
 *
 * @param value The value to write
 */
uint8_t motorcontrol_init(uint8_t value) {
#ifdef MOTORCONTROL_AVAILABLE
    if (i2c_start(I2C_ADDR_MOTORCONTROL + I2C_WRITE)) {
        i2c_write(MC_ENABLE);
        i2c_write(value);
        i2c_stop();
        return 1;
    }
#endif /* MOTORCONTROL_AVAILABLE */
    return 0;
}

/**
 * Set the speed of the motors
 *
 * @param motor_a Speed of motor 1
 * @param motor_b Speed of motor 2
 * @param motor_c Speed of motor 3
 * @param motor_d Speed of motor 4
 * @return If the setting was successful
 */
uint8_t motorcontrol_set_motors(uint8_t motor_a, uint8_t motor_b, uint8_t motor_c, uint8_t motor_d) {
#ifdef MOTORCONTROL_AVAILABLE
    if (i2c_start(I2C_ADDR_MOTORCONTROL + I2C_WRITE)) {
        i2c_write(MC_MOTOR_1_SPEED);
        i2c_write(motor_a);
        i2c_write(motor_b);
        i2c_write(motor_c);
        i2c_write(motor_d);
        i2c_stop();
        return 1;
    }
#endif /* MOTORCONTROL_AVAILABLE */
    return 0;
}

/**
 * Reads the values of the RC channels from the motorcontrol unit
 *
 * @param channel_a Value of channel_a
 * @param channel_b Value of channel_b
 * @param channel_c Value of channel_c
 * @param channel_d Value of channel_d
 * @return If the communication was succesful
 */
uint8_t motorcontrol_get_rc_channels(uint8_t *channel_a, uint8_t *channel_b, uint8_t *channel_c, uint8_t *channel_d) {
#ifdef MOTORCONTROL_AVAILABLE
    if (i2c_start(I2C_ADDR_MOTORCONTROL + I2C_WRITE)) {
        i2c_write(MC_RC_CHANNEL_1);
        if (i2c_rep_start(I2C_ADDR_MOTORCONTROL + I2C_READ)) {
            *channel_a = i2c_read_ack();
            *channel_b = i2c_read_ack();
            *channel_c = i2c_read_ack();
            *channel_d = i2c_read_nak();
            i2c_stop();
            return 1;
        }
    }
#endif /* MOTORCONTROL_AVAILABLE */
    return 0;
}

/**
 * Set the speed of the motors and reads the values of the RC channels from the motorcontrol unit
 *
 * @param motor_a Speed of motor 1
 * @param motor_b Speed of motor 2
 * @param motor_c Speed of motor 3
 * @param motor_d Speed of motor 4
 * @param channel_a Value of channel_a
 * @param channel_b Value of channel_b
 * @param channel_c Value of channel_c
 * @param channel_d Value of channel_d
 * @return If the communication was succesful
 */
uint8_t motorcontrol(uint8_t motor_a, uint8_t motor_b, uint8_t motor_c, uint8_t motor_d, uint8_t *channel_a, uint8_t *channel_b, uint8_t *channel_c, uint8_t *channel_d) {
#ifdef MOTORCONTROL_AVAILABLE
    if (i2c_start(I2C_ADDR_MOTORCONTROL + I2C_WRITE)) {
        /* Write the motor value */
        i2c_write(MC_MOTOR_1_SPEED);
        i2c_write(motor_a);
        i2c_write(motor_b);
        i2c_write(motor_c);
        i2c_write(motor_d);
        if (i2c_rep_start(I2C_ADDR_MOTORCONTROL + I2C_READ)) {
            /* Read the RC channels */
            *channel_a = i2c_read_ack();
            *channel_b = i2c_read_ack();
            *channel_c = i2c_read_ack();
            *channel_d = i2c_read_nak();
            i2c_stop();
            return 1;
        }
    }
#endif /* MOTORCONTROL_AVAILABLE */
    return 0;
}
