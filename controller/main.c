/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file 	controller/main.c
 * @brief 	Main program
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "global_def.h"
#include "init.h"
#include "uart.h"
#include "log.h"
#include "rfm12.h"
#include "i2cmaster.h"
#include "mpu6050.h"
#include "motor.h"

#include <avr/io.h>

#ifdef RFM12B_AVAILABLE
/**
 * Received a rf12b packet
 *
 * @param value The packet data
 */
void rfm12_receive(uint8_t value) {

}
#endif /* RFM12B_AVAILABLE */

uint8_t speed = 0x00;

/**
 * The main function.
 */
int main(void) {

    /* Initialization */
    init_qfly();
    motor_set(0x01, 0x01, 0x01, 0x01);
    log_s("up and running\n");

    /* Our loop */
    while (1) {

#ifdef UART_AVAILABLE
        if (uart_rx_ready()) {
            char *c = uart_rx();
            if (c[0]=='+') {
                if (speed<255) {
                    speed++;
                }
                log_s("Speed: ");
                log_i(speed);
                log_s("\n");
                motor_set(speed, speed, speed, speed);
            }
            if (c[0]=='-') {
                if (speed>0) {
                    speed--;
                }
                log_s("Speed: ");
                log_i(speed);
                log_s("\n");
                motor_set(speed, speed, speed, speed);
            }
            if (c[0]=='1') {
                motor_set(0x1A, 0x1A, 0x1A, 0x1A);
            }

        }
#endif /* UART_AVAILABLE */
    }

    /* Finally. (Never ever) */
    return 0;
}
