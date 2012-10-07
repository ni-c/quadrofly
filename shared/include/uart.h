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

#define UART_BUFFER_SIZE	64 /*!< The size of the uart TX and RX buffer */

#ifndef BAUD

#define BAUD 57600UL /*!< UART Baudrate in bit per second */

#endif

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
 * Returns 1 if the TX buffer is empty
 *
 * @return 1 if the TX buffer is empty
 */
uint8_t uart_tx_ready(void);

/**
 * Send an array of chars
 *
 * @param s An array of chars to send
 */
void uart_tx(const char *s);

/**
 * Converts the integer value to its HEX representation and sends it
 *
 * @param i The int8_t to send
 */
void uart_tx_int8_t(int8_t i);

/**
 * Converts the integer value to its HEX representation and sends it
 *
 * @param i The uint8_t to send
 */
void uart_tx_uint8_t(uint8_t i);
/**
 * Converts the integer value to its HEX representation and sends it
 *
 * @param i The int16_t to send
 */
void uart_tx_int16_t(int16_t i);
/**
 * Converts the integer value to its HEX representation and sends it
 *
 * @param i The uint16_t to send
 */
void uart_tx_uint16_t(uint16_t i);

#endif /* UART_H_ */
