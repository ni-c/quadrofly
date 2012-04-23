/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file 	/quadrofly/controller/lib/millis.c/millis.c
 * @brief   Counting milliseconds since start
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 22, 2012
 */

#include "main.h"
#include "millis.h"

#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/io.h>

uint32_t ms = 0;

/**
 * Timer 0 interrupt
 */
ISR (TIMER0_COMPA_vect)
{
    ms++;
}

/**
 * Initializes the millisecond counter
 */
void millis_init(void) {
    TCCR0A = (1<<WGM01);
    TCCR0B |= (1<<CS02);
    OCR0A = MS_STEPS;
    TIMSK0 |= (1<<OCIE0A);
}

/**
 * Returns the milliseconds since the start of the microcontroller
 *
 * @return The milliseconds since the start of the microcontroller
 */
uint64_t millis(void) {
    return ms;
}
