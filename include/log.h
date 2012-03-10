/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	debug.h
 * @brief 	Debug handling
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 7, 2012
 */

#ifndef DEBUG_H_
#define DEBUG_H_

/**
 * Poll the specified char out the debug port.
 *
 * @param c The specified char
 */
void debug_putc(const unsigned char c);

/**
 * Poll the specified string out the debug port.
 *
 * @param s The specified string
 */
void debug_puts(const char *s);

#endif /* DEBUG_H_ */
