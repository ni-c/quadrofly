/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	cpu/main.h
 * @brief 	Global header file.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#ifndef MAIN_H_
#define MAIN_H_

#ifndef F_CPU
#define F_CPU 1000000UL /*!< Set CPU clock */
#endif

//#define SIMULAVR_AVAILABLE /*!< Compile for the simulavr simulator */
#define LOG_AVAILABLE /*!< Is Logging available */

#define UART_AVAILABLE /*!< Is UART available */
#define BAUD 9600UL /*!< UART Baudrate in bit per second */

#define I2C_SLAVE_AVAILABLE /*!< I2C is available as slave */

#define RFM12B_AVAILABLE /*!< Is RFM12B available */
#define SNAP_AVAILABLE /*!< Is Snap protocol available */

#define MOTOR_AVAILABLE /*!< Is Motor control available */

/* Dependencies */
#ifdef SIMULAVR_AVAILABLE
#undef UART_AVAILABLE /*!< UART doesn't work if the software runs on simulavr, deactivate it */
#endif /* SIMULAVR_AVAILABLE */

#ifdef SNAP_AVAILABLE
#define SNAP_STATIC_ADDR	0x0001
#endif

#endif
