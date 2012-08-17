/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file 	controller/lib/kalman.c
 * @brief 	Kalman Filter
 * @ingroup controller
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 28, 2012
 */

#include "main.h"
#include "kalman.h"

float kalman_angle[3] = {0, 0, 0}; /*!< Kalman filter help var */
float kalman_bias[3] = {0, 0, 0}; /*!< Kalman filter help var */
float kalman_P_00[3] = {0, 0, 0}; /*!< Kalman filter help var */
float kalman_P_01[3] = {0, 0, 0}; /*!< Kalman filter help var */
float kalman_P_10[3] = {0, 0, 0}; /*!< Kalman filter help var */
float kalman_P_11[3] = {0, 0, 0}; /*!< Kalman filter help var */
float kalman_y[3] = {0, 0, 0}; /*!< Kalman filter help var */
float kalman_S[3] = {0, 0, 0}; /*!< Kalman filter help var */
float kalman_K_0[3] = {0, 0, 0}; /*!< Kalman filter help var */
float kalman_K_1[3] = {0, 0, 0}; /*!< Kalman filter help var */

/**
 * Kalman filter
 *
 * @param acc The acc value to use for the kalman filter
 * @param gyro The gyro value to use for the kalman filter
 * @param looptime The looptime since the last call
 * @param key A unique key to identify the kalman filter (0..2)
 */
float kalmanCalculate(float acc, float gyro, uint8_t looptime, uint8_t key) {

    float kalman_dt = (float)looptime/1000;

    kalman_angle[key] += kalman_dt * (gyro - kalman_bias[key]);
    kalman_P_00[key] += -kalman_dt * (kalman_P_10[key] + kalman_P_01[key]) + Q_ANGLE * kalman_dt;
    kalman_P_01[key] += -kalman_dt * kalman_P_11[key];
    kalman_P_10[key] += -kalman_dt * kalman_P_11[key];
    kalman_P_11[key] += +Q_GYRO * kalman_dt;

    kalman_y[key] = acc - kalman_angle[key];
    kalman_S[key] = kalman_P_00[key] + R_ANGLE;
    kalman_K_0[key] = kalman_P_00[key] / kalman_S[key];
    kalman_K_1[key] = kalman_P_10[key] / kalman_S[key];

    kalman_angle[key] += kalman_K_0[key] * kalman_y[key];
    kalman_bias[key] += kalman_K_1[key] * kalman_y[key];
    kalman_P_00[key] -= kalman_K_0[key] * kalman_P_00[key];
    kalman_P_01[key] -= kalman_K_0[key] * kalman_P_01[key];
    kalman_P_10[key] -= kalman_K_1[key] * kalman_P_00[key];
    kalman_P_11[key] -= kalman_K_1[key] * kalman_P_01[key];

    return kalman_angle[key];
}
