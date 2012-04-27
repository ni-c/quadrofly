/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	motorcontrol/lib/motor.c
 * @brief 	Library to control the motor. The motors are attached to the ports PB3, PB4, PD6 and PD7.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 10, 2012
 */
#include "main.h"
#include "motorcontrol.h"
#include "motor.h"
#include "i2cslave.h"

#ifdef MOTOR_AVAILABLE

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

int16_t pwm_cnt; /*!< counter for the actual position in the PWM cycle */

uint8_t tmp; /*!< tmp value */

/**
 * Timer Compare Interrupt
 */
ISR (TIMER0_COMPA_vect) {
    pwm_cnt++;

    // 1 ms
    if (pwm_cnt >= PWM_MAX) {
        PORTB |= ((1 << PB1) | (1 << PB2));
        PORTD |= ((1 << PD5) | (1 << PD6));
        pwm_cnt = 0;
    } else if ((pwm_cnt <= NO_THROTTLE + 255) && (pwm_cnt >= NO_THROTTLE)){
        tmp = pwm_cnt - NO_THROTTLE;
        if (tmp == i2c_buffer[MC_MOTOR_1_SPEED]) {
            PORTD &= ~(1 << PD6);
        }
        if (tmp == i2c_buffer[MC_MOTOR_2_SPEED]) {
            PORTB &= ~(1 << PB1);
        }
        if (tmp == i2c_buffer[MC_MOTOR_3_SPEED]) {
            PORTD &= ~(1 << PD5);
        }
        if (tmp == i2c_buffer[MC_MOTOR_4_SPEED]) {
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

    // Set ports PD6 (OC0A), PB1 (OC1A), PB2 (OC0B) and PB1 (OC1B) to output
    DDRB |= ((1 << DDB1) | (1 << DDB2));
    DDRD |= ((1 << DDD5) | (1 << DDD6));

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
