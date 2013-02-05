/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	controller/init.c
 * @brief 	Global initialization file.
 * @ingroup controller
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "global_def.h"
#include "motorcontrol.h"
#include "motorcom.h"
#include "log.h"
#include "uart.h"
#include "i2cmaster.h"
#include "rfm12.h"
#include "mpu6050.h"
#include "millis.h"
#include "pid.h"

#include <avr/interrupt.h>
#include <util/delay.h>

/**
 * Wait between initializations
 */
static void wait(void) {
    _delay_ms(10);
}

/**
 * Quadrofly initialization
 */
void init_qfly(void) {

    uint8_t ready = 1; /*!< If the initialization was successful */

    /* Initialize PID values */
    pid_init();
    wait();

    /* Enable global interrupts */
    sei();
    wait();

    /* Initialize millis */
    millis_init();
    wait();

#ifdef LED_AVAILABLE

    /* Set pins to output */DDRB |= (1 << DD0) | (1 << DD1);
    PORTB |= (1 << PB1);
    wait();

#endif /* LED_AVAILABLE */

#ifdef UART_AVAILABLE

    /* Initialize UART */
    uart_init();
    log_s("\n\n");
    log_s("uart ... ok\n");
    wait();

#endif /* UART_AVAILABLE */

#ifdef I2C_MASTER_AVAILABLE

    /* Initialize I2C */
    log_s("i2c ...");
    i2c_init();
    wait();

#endif /* I2C_MASTER_AVAILABLE */

#ifdef RFM12B_AVAILABLE

    /* Initialize RFM12B*/
    rfm12_init();  // Initialize module
    rfm12_setfreq(RF12FREQ(433.92));  // set frequency to 433,92MHz
    rfm12_setbandwidth(4, 1, 4);  // 200kHz bandwith, -6dB, DRSSI threshold: -79dBm
    rfm12_setbaud(19200);  // 19200 BAUD
    rfm12_setpower(0, 6);  // 1mW power, 120kHz frequency shift

    /*Initialize interrupt */DDRD |= ~(1 << NIRQ);  // set NIRQ to input
    EICRA |= (1 << ISC01);  // The falling edge of INT0 generates an interrupt request
    EIMSK |= (1 << INT0);  // enable INT0 interrupt

    /**
     * Enable RX
     */
    rfm12_rx_on();
    wait();

#endif /* RFM12B_AVAILABLE */

#ifdef MOTORCONTROL_AVAILABLE

    /* Initialize motorcontrol */
    log_s("motorcontrol ... ");
    if (motorcontrol_init(0x01)) {
        log_s("ok\n");
    } else {
        log_s("failed\n");
        ready = 0;
    }
    wait();

#endif /* MOTORCONTROL_AVAILABLE */

#ifdef MPU6050_AVAILABLE

    /* Initialize MPU6050 */
    if (!mpu6050_init()) {
        ready = 0;
    }
    wait();

#endif /* MPU6050_AVAILABLE */

#ifdef LED_AVAILABLE

    /* Initialize LEDs */
    if (ready) {
        PORTB |= (1 << PB0);
        PORTB &= ~(1 << PB1);
    }
    wait();

#endif /* LED_AVAILABLE */

}
