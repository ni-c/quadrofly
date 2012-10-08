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

#include <stdlib.h>
#include <inttypes.h>

#ifdef EEPROM_AVAILABLE
#include <avr/eeprom.h>
#endif /* EEPROM_AVAILABLE */

static int16_t pid_e_sum[4] = { 0, 0, 0, 0 }; /*!< Sum of last errors */
static int8_t pid_e_old[4] = { 0, 0, 0, 0 }; /*!< Last error */

/* PID values */
float pid_p = PID_KP; /*!< Factor P */
float pid_i = PID_KI; /*!< Factor I */
float pid_d = PID_KD; /*!< Factor D */
uint16_t pid_max_error_sum = PID_MAX_ERROR_SUM; /*!< Max errors we sum, more errors will be ignored */
uint8_t pid_error_cap = PID_ERROR_CAP; /*!< An error below this cap invalidates all errors and starts from scratch */

#ifdef EEPROM_AVAILABLE
static float EEMEM ee_pid_p = PID_KP; /*!< EEMEM var for pid_p */
static float EEMEM ee_pid_i = PID_KI; /*!< EEMEM var for pid_i */
static float EEMEM ee_pid_d = PID_KD; /*!< EEMEM var for pid_d */
static uint16_t EEMEM ee_pid_max_error_sum = PID_MAX_ERROR_SUM; /*!< EEMEM var for max_error_sum */
static uint8_t EEMEM ee_pid_error_cap = PID_ERROR_CAP; /*!< EEMEM var for error_cap */
#endif /* EEPROM_AVAILABLE */

/**
 * Initialize PID controller and read settings from EEPROM
 */
void pid_init(void) {
#ifdef EEPROM_AVAILABLE
    /* Read vars from EEPROM */
    pid_p = eeprom_read_float(&ee_pid_p);
    pid_i = eeprom_read_float(&ee_pid_i);
    pid_d = eeprom_read_float(&ee_pid_d);
    pid_max_error_sum = eeprom_read_word(&ee_pid_max_error_sum);
    pid_error_cap = eeprom_read_byte(&ee_pid_error_cap);
#endif /* EEPROM_AVAILABLE */
}

/**
 * Writes the PID variables to the EEPROM
 */
void pid_eeprom_write(void) {
#ifdef EEPROM_AVAILABLE
    eeprom_update_float(&ee_pid_p, pid_p);
    eeprom_update_float(&ee_pid_i, pid_i);
    eeprom_update_float(&ee_pid_d, pid_d);
    eeprom_update_word(&ee_pid_max_error_sum, pid_max_error_sum);
    eeprom_update_byte(&ee_pid_error_cap, pid_error_cap);
#endif /* EEPROM_AVAILABLE */
}

/**
 * PID controller
 *
 * @param target The target value to reach
 * @param actual The actual value
 * @param key A unique key to identify the pid filter (0..3)
 * @return The calculated PID control value
 */
int16_t pid_calculate(int8_t target, int8_t actual, uint8_t key) {
    int16_t e; /*!< Error */

    float Ki; /*!< Temporary Factor I */
    int16_t u_p; /* P value */
    int16_t u_i; /* I value */
    int16_t u_d; /* D value */

    /* Error */
    e = (target - actual);

    /* I part extensions */
    pid_e_sum[key] += e;
    if (abs(e) <= pid_error_cap) {
        Ki = 0;
        pid_e_sum[key] = 0;
    } else {
        Ki = pid_i;
        if (pid_e_sum[key] >= pid_max_error_sum) {
            pid_e_sum[key] = pid_max_error_sum;
        } else if (pid_e_sum[key] <= -pid_max_error_sum) {
            pid_e_sum[key] = -pid_max_error_sum;
        }
    }

    /* P */
    u_p = e * pid_p;

    /* I */
    u_i = pid_e_sum[key] * Ki;

    /* D */
    u_d = (e - pid_e_old[key]) * pid_d;

    pid_e_old[key] = e;

    return actual + (u_p + u_i + u_d);
}
