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

volatile int16_t pid_e_sum;
volatile int16_t pid_e_old;

#define MAX_PID_E_SUM   5000   /*!< Max value for error sum */
#define PID_ERROR_CAP   -10 /*!< Cap that invalidates errors */

/**
 * PID controller
 *
 * @param target The target value to reach
 * @param actual The actual value
 * @param The control value
 */
int16_t pidCalculate(int16_t target, int16_t actual) {
    int16_t u; /*!< Result */
    int16_t e; /*!< Error */

    float Ki; /*!< Temporary Factor I */
    int16_t u_p; /* P value */
    int16_t u_i; /* I value */
    int16_t u_d; /* D value */

    e = (target - actual);
    if ((e > 10000) || (e < -10000)) {
        Ki = 0;
    } else {
        Ki = PID_KI;
    }

    pid_e_sum += e;
    if ((e <= PID_ERROR_CAP) || (e >= PID_ERROR_CAP)) {
        pid_e_sum = 0;
    }

    if (pid_e_sum >= MAX_PID_E_SUM) {
        pid_e_sum = MAX_PID_E_SUM;
    } else if (pid_e_sum <= -MAX_PID_E_SUM) {
        pid_e_sum = -MAX_PID_E_SUM;
    }

    u_p = e * PID_KP;
    u_i = pid_e_sum * Ki;
    u_d = (e - pid_e_old) * PID_KD;
    u = u_p + u_i + u_d;

    pid_e_old = e;

    return u;
}
