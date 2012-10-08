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

/**
 * Poll the specified uint16_t out the log port.
 *
 * @param i The specified uint16_t
 */
void log_uint16_t(uint16_t i) {
#ifdef LOG_AVAILABLE
#ifdef UART_AVAILABLE
    uart_tx_uint16_t(i);
#endif /* UART_AVAILABLE */
#endif /* LOG_AVAILBLE */
}

/**
 * Poll the specified int16_t out the log port.
 *
 * @param i The specified int16_t
 */
void log_int16_t(int16_t i) {
#ifdef LOG_AVAILABLE
#ifdef UART_AVAILABLE
    uart_tx_int16_t(i);
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
#ifdef UART_AVAILABLE
    uart_tx(s);
#endif /* UART_AVAILABLE */
#endif /* LOG_AVAILBLE */
}
