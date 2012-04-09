/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	motorcontrol/main.h
 * @brief 	Global header file.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#ifndef MAIN_H_
#define MAIN_H_

#include "inttypes.h"

#ifndef F_CPU
#define F_CPU 20000000UL /*!< Set CPU clock */
#endif

#define MOTOR_AVAILABLE /*!< If the motor ESCs are available */

#define I2C_SLAVE_AVAILABLE /*!< I2C is available as slave */

//#define SIMULAVR_AVAILABLE /*!< Compile for the simulavr simulator */
//#define LOG_AVAILABLE /*!< Is Logging available */

#endif
