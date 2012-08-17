/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file    controller/include/motorcom.h
 * @brief   Communication with the motorcontrol
 * @ingroup controller
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 19, 2012
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <inttypes.h>

/**
 * Writes value to the ENABLE register of the motorcontrol unit
 *
 * @param value The value to write
 */
uint8_t motorcontrol_init(uint8_t value);

/**
 * Set the speed of the motors
 *
 * @param motor_a Speed of motor A
 * @param motor_b Speed of motor B
 * @param motor_c Speed of motor C
 * @param motor_d Speed of motor D
 * @return If the setting was succesful
 */
uint8_t motorcontrol_set_motors(uint8_t motor_a, uint8_t motor_b, uint8_t motor_c, uint8_t motor_d);

/**
 * Reads the values of the RC channels from the motorcontrol unit
 *
 * @param channel_a Value of channel_a
 * @param channel_b Value of channel_b
 * @param channel_c Value of channel_c
 * @param channel_d Value of channel_d
 * @return If the communication was succesful
 */
uint8_t motorcontrol_get_rc_channels(uint8_t *channel_a, uint8_t *channel_b, uint8_t *channel_c, uint8_t *channel_d);

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
uint8_t motorcontrol(uint8_t motor_a, uint8_t motor_b, uint8_t motor_c, uint8_t motor_d, uint8_t *channel_a, uint8_t *channel_b, uint8_t *channel_c, uint8_t *channel_d);

#endif /* MOTOR_H_ */
