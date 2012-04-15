/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	controller/init.c
 * @brief 	Global initialization file.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "log.h"
#include "uart.h"
#include "i2cmaster.h"
#include "rfm12.h"
#include "mpu6050.h"

#include <avr/interrupt.h>

/**
 * Quadrofly initialization
 */
void init_qfly(void) {

#ifdef LED_AVAILABLE
    // Set pins to output
    DDRB |= (1 << PB0) | (1 << PB1);
    PORTB |= (1 << PB1);
#endif /* LED_AVAILABLE */

#ifdef UART_AVAILABLE
    uart_init();
    log_s("uart initialization ... ok\n");
#endif /* UART_AVAILABLE */

#ifdef I2C_MASTER_AVAILABLE
    /*
     * Initialize I2C
     */
    log_s("i2c initialization ...");
    i2c_init();
    log_s(" ok\n");
#endif /* I2C_MASTER_AVAILABLE */

#ifdef RFM12B_AVAILABLE
    /*
     * Initialize RFM12B
     */
    rfm12_init();  // Initialize module
    rfm12_setfreq(RF12FREQ(433.92));  // set frequency to 433,92MHz
    rfm12_setbandwidth(4, 1, 4);  // 200kHz bandwith, -6dB, DRSSI threshold: -79dBm
    rfm12_setbaud(19200);  // 19200 BAUD
    rfm12_setpower(0, 6);  // 1mW power, 120kHz frequency shift

    log_s("interrupt initialization ...");
    /*
     * Initialize interrupt
     */
    DDRD |= ~(1 << NIRQ);  // set NIRQ to input
    EICRA |= (1 << ISC01);  // The falling edge of INT0 generates an interrupt request
    EIMSK |= (1 << INT0);  // enable INT0 interrupt
    log_s(" ok\n");

    /**
     * Enable RX
     */
    rfm12_rx_on();

#endif /* RFM12B_AVAILABLE */

#ifdef MPU6050_AVAILABLE
    /*
     * Initialize MPU6050
     */
    mpu6050_init();
#endif /* MPU6050_AVAILABLE */

    /**
     * Enable global interrupts
     */
    sei();

#ifdef LED_AVAILABLE
    PORTB &= ~(1 << PB1);
    PORTB |= (1 << PB0);
#endif /* LED_AVAILABLE */

}
