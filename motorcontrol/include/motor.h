/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	motorcontrol/include/motor.h
 * @brief 	Library to control the motor. The motors are attached to the ports PB3, PB4, PD6 and PD7.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 10, 2012
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>
#include <avr/io.h>

#define TIMER_COMPARE   (F_CPU / 250000) /*!< Timer compare */

#define PWM_CYCLE       200 /*!< PWM Cycle in Hz */
#define PWM_MAX         (100000 / PWM_CYCLE) /*!< Max PWM length (~ 500) */
#define NO_THROTTLE     235 /*!< PWM length for no throttle, should be around millisecond (~ 250) */

/**
 * Motor initialization
 */
void motor_init(void);

#endif /* MOTOR_H_ */
