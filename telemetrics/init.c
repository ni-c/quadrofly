/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	telemetrics/init.c
 * @brief 	Global initialization file.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 *
 * @defgroup telemetrics Telemetrics
 */
#include "main.h"
#include "global_def.h"
#include "i2cslave.h"
#include "uart.h"
#include "rfm12.h"

#include <avr/interrupt.h>

/**
 * Quadrofly initialization
 */
void init_qfly(void) {

#ifdef I2C_SLAVE_AVAILABLE
    /*
     * Initialize I2C slave
     */
    i2c_slave_init(I2C_ADDR_MOTORCONTROL);
#endif /* I2C_SLAVE_AVAILABLE */

#ifdef UART_AVAILABLE
    /*
     * Initialize UART
     */
    uart_init();
#endif /* UART_AVAILABLE */

#ifdef RFM12B_AVAILABLE
    /*
     * Initialize RFM12B
     */
    rfm12_init();  // Initialize module
    rfm12_setfreq(RF12FREQ(433.92));  // set frequency to 433,92MHz
    rfm12_setbandwidth(4, 1, 4);  // 200kHz bandwith, -6dB, DRSSI threshold: -79dBm
    rfm12_setbaud(19200);  // 19200 BAUD
    rfm12_setpower(0, 6);  // 1mW power, 120kHz frequency shift

    /*
     * Initialize interrupt
     */
    DDRD |= ~(1 << NIRQ);  // set NIRQ to input
    EICRA |= (1 << ISC01);  // The falling edge of INT0 generates an interrupt request
    EIMSK |= (1 << INT0);  // enable INT0 interrupt

    /**
     * Enable RX
     */
    rfm12_rx_on();

#endif /* RFM12B_AVAILABLE */

    /*
     * Enable global interupts
     */
    sei();

}
