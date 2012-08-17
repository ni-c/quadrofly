/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * I2C-Register:
 *
 * <pre>
 * Address:     Value:
 * 0x00         0x01 to enable
 * 0x01         Speed for motor 1
 * 0x02         Speed for motor 2
 * 0x03         Speed for motor 3
 * 0x04         Speed for motor 4
 * 0x05         RC Channel 1
 * 0x06         RC Channel 2
 * 0x07         RC Channel 3
 * 0x08         RC Channel 4
 * 0x09         RC Channel 5
 * 0x10         RC Channel 6
 * 0x11         RC Channel 7
 * 0x12         RC Channel 8
 * </pre>
 *
 * @file 	motorcontrol/main.c
 * @brief 	Main program
 * @defgroup motorcontrol Motorcontrol
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "global_def.h"
#include "motorcontrol.h"
#include "init.h"
#include "motor.h"
#include "i2cslave.h"

#include <avr/delay.h>

/**
 * interrupt funcition handling received bytes
 *
 * @param addr The address of the last received bytes
 */
void i2c_receive(uint8_t *addr) {
}

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
        if (i2c_buffer[MC_RC_CHANNEL_1]!=0) {
            PORTD &= ~(1 << PD0);
        }
    }

    /* Finally. (Never ever) */
    return 0;
}
