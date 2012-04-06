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

    _delay_ms(100);

    /* Our loop */
    while (1) {

        int16_t buffer[7];

        mpu6050_getall(&buffer[0], &buffer[1], &buffer[2], &buffer[3], &buffer[4], &buffer[5], &buffer[6]);

        for (int i = 0; i < 7; i++) {
            uart_tx_int16_t(buffer[i]);
            uart_tx(", ");
        }
        uart_tx("\n");

        /* Wait 200ms */
        _delay_ms(200);

#ifdef I2C_SLAVE_AVAILABLE
        if (i2c_rx_ready()) {
#ifdef UART_AVAILABLE
            uart_tx_uint8_t(i2c_rx_buffer[0]);
            uart_tx_uint8_t(i2c_rx_buffer[1]);
            uart_tx_uint8_t(i2c_rx_buffer[2]);
            uart_tx_uint8_t(i2c_rx_buffer[3]);
            uart_tx_uint8_t(i2c_rx_buffer[4]);
            uart_tx_uint8_t(i2c_rx_buffer[5]);
            uart_tx_uint8_t(i2c_rx_buffer[6]);
            uart_tx_uint8_t(i2c_rx_buffer[7]);
            uart_tx("\n");
#endif /* UART AVAILABLE */
        }
#endif /* I2C_SLAVE_AVAILABLE */

#ifdef UART_AVAILABLE
/*        if (uart_rx_ready()) {
            uart_tx("Echo: ");
            uart_tx(uart_rx());
            uart_tx("\n");
        }
        */
#endif /* UART_AVAILABLE */
    }

    /* Finally. (Never ever) */
    return 0;
}
