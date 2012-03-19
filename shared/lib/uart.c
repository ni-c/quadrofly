/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	uart.c
 * @brief 	Helper methods for UART
 * @author 	Willi Thiel (thiel.willi@web.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "uart.h"

#ifdef UART_AVAILABLE
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>
#include <stdio.h>

struct {
	unsigned rx :1; /*<! If there is data in the RX buffer */
	unsigned tx :1; /*<! If there is data in the TX buffer */
} uart_flag;

char uart_tx_buffer[UART_BUFFER_SIZE]; /*!< UART send buffer */

char uart_rx_buffer[UART_BUFFER_SIZE]; /*!< UART receive buffer */

#endif /* UART_AVAILABLE */

#ifdef UART_AVAILABLE
/**
 * UART TX data register empty interrupt. Writes the data from the UART buffer
 */
ISR( USART_UDRE_vect) {

	static char* uart_tx_p = uart_tx_buffer; /*!< Pointer to TX buffer */

	uint8_t data; /*!< Char to send */

	data = *uart_tx_p++; // get next char and increase pointer

	if (data == 0) {
		UCSR0B &= ~(1 << UDRIE0); // disable UDRE interrupt
		uart_tx_p = uart_tx_buffer; // reset pointer
		uart_flag.tx = 1; // TX finished
	} else
		UDR0 = data;
}
#endif /* UART_AVAILABLE */

#ifdef UART_AVAILABLE
/**
 * UART RX complete interrupt. Reads the data from the UART
 */
ISR( USART_RX_vect) {

	static uint8_t uart_rx_cnt; /*!< counter for received chars */
	uint8_t data; /*!< received char */

	data = UDR0; // Read data from UART

	if (!uart_flag.rx) {
		// end of string? (return)
		if (data == '\r') {
			uart_rx_buffer[uart_rx_cnt] = 0; // terminate buffer
			uart_flag.rx = 1; // buffer full
			uart_rx_cnt = 0; // reset counter
		} else if (uart_rx_cnt < (UART_BUFFER_SIZE - 1)) {
			uart_rx_buffer[uart_rx_cnt] = data;
			uart_rx_cnt++;
		}
	}
}
#endif /* UART_AVAILABLE */

/**
 * UART Initialization
 */
void uart_init(void) {

#ifdef UART_AVAILABLE

	uart_flag.tx = 1;
	uart_flag.rx = 0;

	UBRR0H = UBRRH_VALUE; // Set calculated baud rate for HIGH port
	UBRR0L = UBRRL_VALUE; // Set calculated baud rate for LOW port

#if USE_2X
	UCSR0A |= (1 << U2X0); // Use 2X mode if available
#else
	UCSR0A &= ~(1 << U2X0); // and not if 2X mode is not available
#endif /* USE_2X */
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // enable UART receiver and transmitter, enable receive-interrupt.
#endif /* UART_AVAILABLE */
}

/**
 * append data to the tx buffer
 *
 * @param data The data to append
 */
void tx_buffer(const char *data) {

#ifdef UART_AVAILABLE
	if (uart_flag.tx == 1) {
		strcpy(uart_tx_buffer, data); // copy data to TX buffer
		uart_flag.tx = 0; // remove TX flag
		UCSR0B |= (1 << UDRIE0); // enable UDRE interrupt
	} else {
		strcat(uart_tx_buffer, data); // add the new data
	}
#endif /* UART_AVAILABLE */
}

/**
 * Returns the content of the UART RX buffer and resets the buffer
 */
char* uart_rx(void) {

#ifdef UART_AVAILABLE
	uart_flag.rx = 0;
	return uart_rx_buffer;
#else
	return "\0";
#endif /* UART_AVAILABLE */
}

/**
 * Returns 1 if the UART RX buffer is ready to read
 *
 * @return 1 if the UART RX buffer is ready to read
 */
uint8_t uart_rx_ready(void) {

#ifdef UART_AVAILABLE
	return uart_flag.rx;
#else
	return 0;
#endif /* UART_AVAILABLE */
}

/**
 * Send an array of chars
 *
 * @param s An array of chars to send
 */
void uart_tx(const char *s) {

#ifdef UART_AVAILABLE
	tx_buffer(s);
#endif /* UART_AVAILABLE */
}

/**
 * Converts the integer value to its HEX representation and sends it
 *
 * @param i The uint8_t to send
 */
void uart_tx_i(uint8_t i) {
#ifdef UART_AVAILABLE
	char buffer[2];
	sprintf(buffer, "%02X", i);
	uart_tx(buffer);
#endif
}
