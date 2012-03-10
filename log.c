/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	log.c
 * @brief 	Log helper class
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 7, 2012
 */
#include "main.h"
#include "uart.h"

#ifdef SIMULAVR_AVAILABLE

#define SIMULAVR_PORT (*((volatile char *)0x20)) /*!< This port correponds to the "-W 0x20,-" command line option. */

#endif

/**
 * Poll the specified char out the log port.
 *
 * @param c The specified char
 */
void log_putc(const unsigned char c) {

#ifdef SIMULAVR_AVAILABLE
	/* write char to special output port */
	SIMULAVR_PORT = c;
#endif
#ifdef UART_AVAILABLE
	/* write char to UART */
	uart_putc(c);
#endif
}

/**
 * Poll the specified string out the log port.
 *
 * @param s The specified string
 */
void log_puts(const char *s) {

#ifdef SIMULAVR_AVAILABLE
	while (*s) {
		log_putc(*s);
		s++;
	}
#endif
#ifdef UART_AVAILABLE
	uart_puts(*s);
#endif
}
