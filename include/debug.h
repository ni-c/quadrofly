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
 * Poll the specified string out the debug port.
 *
 * @param str The specified string
 */
void debug_puts(const char *str);

#endif /* DEBUG_H_ */
