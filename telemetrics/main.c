/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	telemetrics/main.c
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

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

/**
 * Received a rfm12 packet
 *
 * @param value The packet data
 */
void rfm12_receive(uint8_t *value) {

#ifdef UART_AVAILABLE
    uart_tx_uint8_t(value[1]);
    uart_tx_uint8_t(value[2]);
    uart_tx_uint8_t(value[3]);
    uart_tx_uint8_t(value[4]);
    uart_tx("\n");
#endif /* UART_AVAILABLE */
}

/**
 * The main function.
 */
int main(void) {

    /* Initialization */
    init_qfly();
    log_s("receiver initialization ... ok\n");

    /* Our loop */
    while (1) {

        /* Wait 100ms */
        _delay_ms(100);

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
