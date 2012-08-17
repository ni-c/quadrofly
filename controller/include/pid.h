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

#define PID_KP  0.2     /*!< Factor P */
#define PID_KI  0.03    /*!< Factor I */
#define PID_KD  0.05    /*!< Factor D */

/**
 * PID controller
 *
 * @param target The target value to reach
 * @param actual The actual value
 * @param The control value
 */
int16_t pidCalculate(int16_t target, int16_t actual);

#endif /* PID_H_ */
