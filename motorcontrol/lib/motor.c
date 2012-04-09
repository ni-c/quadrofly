/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	motorcontrol/lib/motor.c
 * @brief 	Library to control the motor. The motors are attached to the ports PB3, PB4, PD6 and PD7.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 10, 2012
 */
#include "main.h"
#include "motor.h"

#ifdef MOTOR_AVAILABLE

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t motor[4]; /*!< speed values for the motors */

int16_t pwm_cnt; /*!< counter for the actual position in the PWM cycle */

uint8_t tmp; /*!< tmp value */

/**
 * Timer Compare Interrupt
 */
ISR (TIMER0_COMPA_vect) {
    pwm_cnt++;

    // 1 ms
    if (pwm_cnt == PWM_MAX) {
        PORTB |= ((1 << PB1) | (1 << PB2));
        PORTD |= ((1 << PD5) | (1 << PD6));
        pwm_cnt = 0;
    } else if ((pwm_cnt <= NO_THROTTLE + 255) && (pwm_cnt >= NO_THROTTLE)){
        tmp = pwm_cnt - NO_THROTTLE;
        if (tmp == motor[0]) {
            PORTD &= ~(1 << PD6);
        }
        if (tmp == motor[1]) {
            PORTB &= ~(1 << PB1);
        }
        if (tmp == motor[2]) {
            PORTD &= ~(1 << PD5);
        }
        if (tmp == motor[3]) {
            PORTB &= ~(1 << PB2);
        }
    }
}
#endif /* MOTOR_AVAILABLE */

/**
 * Motor initialization
 */
void motor_init(void) {

#ifdef MOTOR_AVAILABLE

    // Initialization
    pwm_cnt = 0;
    motor[0] = 0;
    motor[1] = 0;
    motor[2] = 0;
    motor[3] = 0;

    // Set ports PD6 (OC0A), PB1 (OC1A), PB2 (OC0B) and PB1 (OC1B) to output
    DDRB |= ((1 << PB1) | (1 << PB2));
    DDRD |= ((1 << PD5) | (1 << PD6));

    // CTC Modus
    TCCR0A = (1 << WGM01);

    // No prescaler
    TCCR0B |= (1 << CS00);

    // Output-Compare
    OCR0A = TIMER_COMPARE;

    // Enable interrupts
    TIMSK0 |= (1 << OCIE0A);
#endif /* MOTOR_AVAILABLE */
}

/**
 * Set the motor with the given nr to the given speed
 *
 * @param nr The nr (0-3) of the motor to set
 * @param speed The speed (0-255) of the motor
 */
void motor_set(uint8_t nr, uint8_t speed) {
    motor[nr] = speed;
}

