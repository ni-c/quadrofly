/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file 	controller/lib/pid.c
 * @brief 	PID controller
 * @ingroup controller
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 29, 2012
 */

#include "main.h"
#include "pid.h"

#include <inttypes.h>

volatile int16_t pid_e_sum[4] = {0, 0, 0, 0};
volatile int16_t pid_e_old[4] = {0, 0, 0, 0};

#define MAX_PID_E_SUM   500   /*!< Max value for error sum */
#define PID_ERROR_CAP   20 /*!< Cap that invalidates errors */

/**
 * PID controller
 *
 * @param target The target value to reach
 * @param actual The actual value
 * @param key A unique key to identify the pid filter (0..3)
 * @return The calculated PID control value
 */
int16_t pid_calculate(int16_t target, int16_t actual, uint8_t key) {
    int16_t e; /*!< Error */

    float Ki; /*!< Temporary Factor I */
    int16_t u_p; /* P value */
    int16_t u_i; /* I value */
    int16_t u_d; /* D value */

    /* Error */
    e = (target - actual);

    /* I part extensions */
    pid_e_sum[key] += e;
    if ((e <= abs(PID_ERROR_CAP)) || (e >= -abs(PID_ERROR_CAP))) {
        Ki = 0;
        pid_e_sum[key] = 0;
    } else {
        Ki = PID_KI;
        if (pid_e_sum[key] >= MAX_PID_E_SUM) {
            pid_e_sum[key] = MAX_PID_E_SUM;
        } else if (pid_e_sum[key] <= -MAX_PID_E_SUM) {
            pid_e_sum[key] = -MAX_PID_E_SUM;
        }
    }

    /* P */
    u_p = e * PID_KP;

    /* I */
    u_i = pid_e_sum[key] * Ki;

    /* D */
    u_d = (e - pid_e_old[key]) * PID_KD;

    pid_e_old[key] = e;

    return actual + (u_p + u_i + u_d);
}
