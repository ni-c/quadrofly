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
#include <avr/io.h>

#define MOTOR_FRONT 	OCR0A /*!< The front motor */
#define MOTOR_RIGHT 	OCR0B /*!< The right motor */
#define MOTOR_REAR 		OCR2B /*!< The rear motor */
#define MOTOR_LEFT 		OCR2A /*!< The left motor */

/**
 * Motor initialization
 */
void motor_init(void);

#endif /* MOTOR_H_ */
