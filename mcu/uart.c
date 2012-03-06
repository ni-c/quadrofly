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

/**
 * UART Initialization
 */
void uart_init(void) {

	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

#if USE_2X
	UCSR0A |= (1 << U2X0);
#else
	UCSR0A &= ~(1 << U2X0);
#endif

	/* Disable UART receiver and transmitter, enable receive-interrupt */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
}

/**
 * Send a char
 *
 * @param c The char to send
 */
int uart_putc(unsigned char c) {

	while (!(UCSR0A & (1 << UDRE0))) /* wait until ready */
	{
	}

	UDR0 = c; /* send */
	return 0;
}

/**
 * Send an array of chars
 *
 * @param s An array of chars to send
 */
void uart_puts(char *s) {

	while (*s) {
		uart_putc(*s);
		s++;
	}
}

#endif
