/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	log.h
 * @brief 	Log handling
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 7, 2012
 */

#ifndef LOG_H_
#define LOG_H_

#include <inttypes.h>

/**
 * Poll the specified uint8_t out the log port.
 *
 * @param i The specified uint8_t
 */
void log_uint8_t(uint8_t i);

/**
 * Poll the specified int8_t out the log port.
 *
 * @param i The specified int8_t
 */
void log_int8_t(int8_t i);

/**
 * Poll the specified uint16_t out the log port.
 *
 * @param i The specified uint16_t
 */
void log_uint16_t(uint16_t i);

/**
 * Poll the specified int16_t out the log port.
 *
 * @param i The specified int16_t
 */
void log_int16_t(int16_t i);

/**
 * Poll the specified string out the debug port.
 *
 * @param s The specified string
 */
void log_s(const char *s);

#endif /* LOG_H_ */
