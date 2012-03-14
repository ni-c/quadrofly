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

/**
 * Poll the specified char out the debug port.
 *
 * @param c The specified char
 */
void log_putc(const unsigned char c);

/**
 * Poll the specified string out the debug port.
 *
 * @param s The specified string
 */
void log_puts(const char *s);

#endif /* LOG_H_ */
