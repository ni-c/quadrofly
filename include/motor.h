/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	motor.h
 * @brief 	Library to control the motor. The motors are attached to the ports PB3, PB4, PD6 and PD7.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 10, 2012
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>

#define MOTOR_FRONT 	1 /*!< The front motor */
#define MOTOR_RIGHT 	2 /*!< The right motor */
#define MOTOR_REAR 		3 /*!< The rear motor */
#define MOTOR_LEFT 		4 /*!< The left motor */

/**
 * Motor initialization
 */
void motor_init(void);

/**
 * Sets the speed of a motor
 *
 * @param nr uint8_t The nr of the motor to set
 * @param value uint8_t The value (0-255) to setc
 */

void motor_set(uint8_t nr, uint8_t value);

#endif /* MOTOR_H_ */
