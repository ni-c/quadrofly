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

#include <stdio.h>
#include <inttypes.h>

#ifdef SIMULAVR_AVAILABLE

#define SIMULAVR_PORT (*((volatile char *)0x20)) /*!< This port correponds to the "-W 0x20,-" command line option. */

#endif

/**
 * Poll the specified uint8 out the log port.
 *
 * @param i The specified uint8
 */
void log_i(uint8_t i) {
#ifdef LOG_AVAILABLE
#ifdef SIMULAVR_AVAILABLE
	char buffer[2];
	char* buffer_p = buffer;
	sprintf(buffer, "%02X", i);
	SIMULAVR_PORT = *buffer_p++;
	SIMULAVR_PORT = *buffer_p;
	SIMULAVR_PORT = 32;
#endif /* SIMULAVAR_AVAILABLE */
#ifdef UART_AVAILABLE
	char buffer[2];
	sprintf(buffer, "%02X", i);
	uart_tx(buffer);
#endif /* UART_AVAILABLE */
#endif /* LOG_AVAILBLE */
}

/**
 * Poll the specified string out the log port.
 *
 * @param s The specified string
 */
void log_s(const char *s) {
#ifdef LOG_AVAILABLE
#ifdef SIMULAVR_AVAILABLE
	for(int i = 0; s[i] != '\0'; i++) {
		SIMULAVR_PORT = s[i];
	}
#endif /* SIMULAVAR_AVAILABLE */
#ifdef UART_AVAILABLE
	uart_tx(s);
#endif /* UART_AVAILABLE */
#endif /* LOG_AVAILBLE */
}
