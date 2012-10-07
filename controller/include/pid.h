/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file 	controller/include/pid.h
 * @brief 	PID Controller
 * @ingroup controller
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 29, 2012
 */

#ifndef PID_H_
#define PID_H_

#include <inttypes.h>

#define PID_KP  0.3     /*!< Factor P */
#define PID_KI  0.04    /*!< Factor I */
#define PID_KD  0.07    /*!< Factor D */

/**
 * PID controller
 *
 * @param target The target value to reach
 * @param actual The actual value
 * @param key A unique key to identify the pid filter (0..3)
 * @return The calculated PID control value
 */
int16_t pid_calculate(int16_t target, int16_t actual, uint8_t key);

#endif /* PID_H_ */
