/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file 	/shared/include/millis.h
 * @brief 	Counting milliseconds since start
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 23, 2012
 */

#ifndef MILLIS_H_
#define MILLIS_H_

#include <inttypes.h>

#define MS_STEPS    F_CPU / 256000 - 1 /*!< Timer steps to count a millisecond */

/**
 * Initializes the millisecond counter
 */
void millis_init(void);

/**
 * Returns the milliseconds since the start of the microcontroller
 *
 * @return The milliseconds since the start of the microcontroller
 */
uint64_t millis(void);

#endif /* MILLIS_H_ */
