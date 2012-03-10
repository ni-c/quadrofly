/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	motor.c
 * @brief 	Library to control the motor. The motors are attached to the ports PB3, PB4, PD6 and PD7.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 10, 2012
 */
#include "main.h"
#include "motor.h"
#include <stdint.h>

#ifdef MOTOR_AVAILABLE
#include <avr/io.h>
#include <avr/interrupt.h>
#endif /* MOTOR_AVAILABLE */

/**
 * Motor initialization
 */
void motor_init(void) {

#ifdef MOTOR_AVAILABLE

	// Set ports PB3 (OC0A), PB4 (OC0B), PD6 (OC2B) and PD7 (OC2A) to output
	DDRB &= ~((1 << PB3) | (1 << PB4));
	DDRD &= ~((1 << PD6) | (1 << PD7));

	// PWM Timer 0 - 76.29 Hz @ 20 MHz
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00); // NON Inverted Fast PWM
	TCCR0B |= (1 << CS02) | (1 << CS01) | (1 << CS00); // Prescaler = 1024, Mode 3

	// PWM Timer 2 - 76.29 Hz @ 20 MHz
	TCCR2A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11) | (1 << WGM10); // NON Inverted Fast PWM
	TCCR2B |= (1 << CS12) | (1 << CS11) | (1 << CS10); // Prescaler = 1024, Mode 3

	OCR0A = 0; // Stop Motor 1
	OCR0B = 0; // Stop Motor 2
	OCR2B = 0; // Stop Motor 3
	OCR2A = 0; // Stop Motor 4

#endif /* MOTOR_AVAILABLE */
}

/**
 * Sets the speed of a motor
 *
 * @param nr uint8_t The nr of the motor to set
 * @param value uint8_t The value (0-255) to setc
 */
void motor_set(uint8_t nr, uint8_t value) {

#ifdef MOTOR_AVAILABLE

	switch (nr) {
	case MOTOR_FRONT:
		OCR0A = value;
		break;
	case MOTOR_RIGHT:
		OCR0B = value;
		break;
	case MOTOR_REAR:
		OCR2B = value;
		break;
	case MOTOR_LEFT:
		OCR2A = value;
		break;
	default:
		break;
	}
#endif /* MOTOR_AVAILABLE */
}
