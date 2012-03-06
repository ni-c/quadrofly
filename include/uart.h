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

/**
 * UART Initialization
 */
void uart_init(void);

/**
 * Send a char
 *
 * @param c The char to send
 */
int uart_putc(unsigned char c);

/**
 * Send an array of chars
 *
 * @param s An array of chars to send
 */
void uart_puts(char *s);

#endif /* UART_H_ */
