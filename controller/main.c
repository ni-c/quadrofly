/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file 	controller/main.c
 * @brief 	Main program
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "init.h"
#include "uart.h"
#include "log.h"
#include "rfm12.h"
#include "i2cmaster.h"
#include "mpu6050.h"

#include <avr/io.h>
#include <util/delay.h>

/**
 * Received a rf12b packet
 *
 * @param value The packet data
 */
void rfm12_receive(uint8_t value) {

}

/**
 * The main function.
 */
int main(void) {

    /* Initialization */
    init_qfly();
    log_s("initialization finished\n");

    _delay_ms(500);

    /* Our loop */
    while (1) {


        uint8_t v1[5];
        v1[0] = 0x54;
        uint8_t v2[5];
        v2[0] = 0x54;

        log_i(mpu6050_get(MPU6050_TEMP_OUT));
        log_s("\n");

        /* Wait 500ms */
        _delay_ms(500);

#ifdef I2C_SLAVE_AVAILABLE
        if (i2c_rx_ready()) {
#ifdef UART_AVAILABLE
            uart_tx_i(i2c_rx_buffer[0]);
            uart_tx_i(i2c_rx_buffer[1]);
            uart_tx_i(i2c_rx_buffer[2]);
            uart_tx_i(i2c_rx_buffer[3]);
            uart_tx_i(i2c_rx_buffer[4]);
            uart_tx_i(i2c_rx_buffer[5]);
            uart_tx_i(i2c_rx_buffer[6]);
            uart_tx_i(i2c_rx_buffer[7]);
            uart_tx("\n");
#endif /* UART AVAILABLE */
        }
#endif /* I2C_SLAVE_AVAILABLE */

#ifdef UART_AVAILABLE
        if (uart_rx_ready()) {
            uart_tx("Echo: ");
            uart_tx(uart_rx());
            uart_tx("\n");
        }
#endif /* UART_AVAILABLE */
    }

    /* Finally. (Never ever) */
    return 0;
}
