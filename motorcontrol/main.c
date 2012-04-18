/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	motorcontrol/main.c
 * @brief 	Main program
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "global_def.h"
#include "init.h"
#include "motor.h"
#include "i2cslave.h"

#include <avr/delay.h>

uint8_t running = 0; /*!< If one of the motors is set to another speed then 0, this will change to 1 */

/**
 * The main function.
 */
int main(void) {

    /* LED on */
    DDRD |= (1 << DDD0);
    PORTD |= (1 << PD0);

    /* Initialization */
    init_qfly();

    /* Our loop */
    while (1) {
        /* Turn LED off if we get a signal */
        if ((running==0) && (!((i2c_buffer[0]==0) && (i2c_buffer[1]==0) && (i2c_buffer[2]==0) && (i2c_buffer[3]==0)))) {
            /* LED off */
            PORTD &= ~(1 << PD0);
            running=1;
        }
    }

    /* Finally. (Never ever) */
    return 0;
}
