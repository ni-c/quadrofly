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

    DDRC = 0xff;  // set port c to output (there are our testing leds)

    DDRD &= ~((1 << PD2) | (1 << PD3));  // set ports PD2 and PD3 as input (interrupts)

    /* Initialization */
    init_qfly();
    log_s("initialization... ok\n");

    /* Our loop */
    while (1) {

        /* Wait 500ms */
        _delay_ms(500);
        PORTC |= (1 << PC1);  // enable LED 1

        uint8_t v1[5];
        v1[0] = 0x54;
        uint8_t v2[5];
        v2[0] = 0x54;

        i2c_start(0x68 + I2C_WRITE);
        v1[1] = i2c_write(0x3B);
        i2c_rep_start(0x68 + I2C_READ);
        v1[2] = i2c_readNak();
        //v1[3] = i2c_readNak();
        //v1[4] = i2c_readNak();
        //v2[1] = i2c_readNak();
        //v2[2] = i2c_readNak();
        //v2[3] = i2c_readNak();
        //v2[4] = i2c_readNak();
        i2c_stop();

        rfm12_send(v1);

        /* Wait 500ms */
        _delay_ms(100);
        rfm12_send(v2);

        PORTC &= ~(1 << PC1);  // disable LED 1

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
