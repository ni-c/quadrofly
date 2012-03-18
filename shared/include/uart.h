/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	uart.h
 * @brief 	UART control methods
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#ifndef UART_H_
#define UART_H_

#include <inttypes.h>
#include <string.h>

#define UART_BUFFER_SIZE	32 /*!< The size of the uart TX and RX buffer */

/**
 * UART Initialization
 */
void uart_init(void);

/**
 * Returns the content of the UART RX buffer and resets the buffer
 */
char* uart_rx(void);

/**
 * Returns 1 if the UART RX buffer is ready to read
 *
 * @return 1 if the UART RX buffer is ready to read
 */
uint8_t uart_rx_ready(void);

/**
 * Send an array of chars
 *
 * @param s An array of chars to send
 */
void uart_tx(const char *s);

#endif /* UART_H_ */
