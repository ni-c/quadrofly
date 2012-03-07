/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	uart.c
 * @brief 	Helper methods for UART
 * @author 	Willi Thiel (thiel.willi@web.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"

#ifdef UART_AVAILABLE
#include <avr/io.h>
#include <util/setbaud.h>
#endif /* UART_AVAILABLE */
/**
 * UART Initialization
 */
void uart_init(void) {

#ifdef UART_AVAILABLE
	/* Set calculated baud rate for HIGH port */
	UBRR0H = UBRRH_VALUE;
	/* Set calculated baud rate for LOW port */
	UBRR0L = UBRRL_VALUE;

#if USE_2X
	/* Use 2X mode if available */
	UCSR0A |= (1 << U2X0);
#else
	/* and not if 2X mode is not available */
	UCSR0A &= ~(1 << U2X0);
#endif /* USE_2X */

	/* Disable UART receiver and transmitter, enable receive-interrupt */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
#endif /* UART_AVAILABLE */
}

/**
 * Send a char
 *
 * @param c The char to send
 */
void uart_putc(const unsigned char c) {

#ifdef UART_AVAILABLE
	/* wait until UART ready */
	while (!(UCSR0A & (1 << UDRE0)))
	{
	}

	/* write c to uart port */
	UDR0 = c;
#endif /* UART_AVAILABLE */
}

/**
 * Send an array of chars
 *
 * @param s An array of chars to send
 */
void uart_puts(const char *s) {

#ifdef UART_AVAILABLE
	while (*s) {
		uart_putc(*s);
		s++;
	}
#endif /* UART_AVAILABLE */
}
