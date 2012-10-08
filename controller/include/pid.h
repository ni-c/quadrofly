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

#define PID_KP              0.6     /*!< Initial factor P */
#define PID_KI              0.0     /*!< Initial factor I */
#define PID_KD              0.3     /*!< Initial factor D */

#define PID_MAX_ERROR_SUM   500     /*!< Initial max value for error sum */
#define PID_ERROR_CAP       20      /*!< Initial cap that invalidates errors */

/* PID values */
float pid_p;                        /*!< Current factor P */
float pid_i;                        /*!< Current factor I */
float pid_d;                        /*!< Current factor D */
uint16_t pid_max_error_sum;          /*!< Max errors we sum, more errors will be ignored */
uint8_t pid_error_cap;              /*!< An error below this cap invalidates all errors and starts from scratch */

/**
 * Initialize PID controller and read settings from EEPROM
 */
void pid_init(void);

/**
 * Writes the PID variables to the EEPROM
 */
void pid_eeprom_write(void);

/**
 * PID controller
 *
 * @param target The target value to reach
 * @param actual The actual value
 * @param key A unique key to identify the pid filter (0..3)
 * @return The calculated PID control value
 */
int16_t pid_calculate(int8_t target, int8_t actual, uint8_t key);

#endif /* PID_H_ */
