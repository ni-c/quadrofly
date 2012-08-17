/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	motorcontrol/main.h
 * @brief 	Global header file.
 * @ingroup motorcontrol
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#ifndef MAIN_H_
#define MAIN_H_

#include "inttypes.h"

#define I2C_BUFFER_SIZE 16 /*!< Size of the I2C buffer 2..254 */

#ifndef F_CPU
#define F_CPU 20000000UL /*!< Set CPU clock */
#endif

#define MOTOR_AVAILABLE /*!< If the motor ESCs are available */

#define RX_AVAILABLE /*!< If the RX decoder is available */

#define I2C_SLAVE_AVAILABLE /*!< I2C is available as slave */

//#define SIMULAVR_AVAILABLE /*!< Compile for the simulavr simulator */
//#define LOG_AVAILABLE /*!< Is Logging available */

/* Dependencies */
#ifdef MOTOR_AVAILABLE
#define I2C_SLAVE_AVAILABLE
#endif

#endif
