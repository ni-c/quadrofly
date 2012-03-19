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
 * Poll the specified uint8 out the log port.
 *
 * @param s The specified uint8
 */
void log_i(uint8_t i);

/**
 * Poll the specified string out the debug port.
 *
 * @param s The specified string
 */
void log_s(const char *s);

#endif /* LOG_H_ */
