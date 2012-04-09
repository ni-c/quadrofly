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

#define PWM_CYCLE       50 /*!< PWM Cycle in Hz */

#define TIMER_COMPARE   (F_CPU / 100000) /*!< Timer compare */
#define PWM_MAX         (100000 / PWM_CYCLE) /*!< PWM reset value */

#define NO_THROTTLE     80 /*!< PWM length for no throttle */

/**
 * Motor initialization
 */
void motor_init(void);

/**
 * Set the motor with the given nr to the given speed
 *
 * @param nr The nr (0-3) of the motor to set
 * @param speed The speed (0-255) of the motor
 */
void motor_set(uint8_t nr, uint8_t speed);

#endif /* MOTOR_H_ */
