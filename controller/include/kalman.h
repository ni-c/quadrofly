/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file    controller/include/kalman.h
 * @brief   Kalman Filter Implementation
 * @ingroup controller
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 28, 2012
 */

#ifndef KALMAN_H_
#define KALMAN_H_

#include <inttypes.h>

#define Q_ANGLE 0.001 /*<! Q Angle multiplier of the kalman filter */
#define Q_GYRO  0.003 /*<! Q Gyro multiplier of the kalman filter */
#define R_ANGLE 0.03 /*<! R Angle multiplier of the kalman filter */

/**
 * Kalman filter
 *
 * @param acc The acc value to use for the kalman filter
 * @param gyro The gyro value to use for the kalman filter
 * @param looptime The looptime since the last call
 * @param key A unique key to identify the kalman filter (0..2)
 */
float kalman_calculate(float acc, float gyro, uint8_t looptime, uint8_t key);

#endif /* KALMAN_H_ */
