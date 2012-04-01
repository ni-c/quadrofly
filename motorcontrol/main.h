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
#define F_CPU 1000000UL /*!< Set CPU clock */
#endif

#define MOTOR_AVAULABLE /*!< If the motor ESCs are available */

//#define SIMULAVR_AVAILABLE /*!< Compile for the simulavr simulator */
//#define LOG_AVAILABLE /*!< Is Logging available */

#endif
