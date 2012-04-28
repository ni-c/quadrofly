/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file    motorcom.h
 * @brief   Communication with the motorcontrol
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 19, 2012
 */

#ifndef MOTOR_H_
#define MOTOR_H_

/**
 * Writes value to the ENABLE register of the motorcontrol unit
 *
 * @value The value to write
 */
uint8_t motorcontrol_init(uint8_t value);

/**
 * Set the speed of the motors
 *
 * @param a Speed of motor A
 * @param b Speed of motor B
 * @param c Speed of motor C
 * @param d Speed of motor D
 * @return If the setting was succesful
 */
uint8_t motorcontrol_set_motors(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

/**
 * Reads the values of the RC channels from the motorcontrol unit
 *
 * @param channel_a Value of channel_a
 * @param channel_a Value of channel_b
 * @param channel_a Value of channel_c
 * @param channel_a Value of channel_d
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
 * @param channel_a Value of channel_b
 * @param channel_a Value of channel_c
 * @param channel_a Value of channel_d
 * @return If the communication was succesful
 */
uint8_t motorcontrol(uint8_t motor_a, uint8_t motor_b, uint8_t motor_c, uint8_t motor_d, uint8_t *channel_a, uint8_t *channel_b, uint8_t *channel_c, uint8_t *channel_d);

#endif /* MOTOR_H_ */
