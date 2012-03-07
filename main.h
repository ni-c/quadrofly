/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	main.h
 * @brief 	Global header file.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#ifndef MAIN_H_
#define MAIN_H_

#ifndef F_CPU
#define F_CPU 20000000UL /*!< Set CPU clock */
#endif

#define SIMULAVR_AVAILABLE /*!< Compile for the simulavr simulator */

#define UART_AVAILABLE /*!< Is UART available */
#define BAUD 57600UL /*!< UART Baudrate in bit per second */

/* Dependencies */
#ifdef SIMULAVR_AVAILABLE
#undef UART_AVAILABLE /*!< UART doesn't work if the software runs on simulavr, deactivate it */
#endif /* SIMULAVR_AVAILABLE */

#endif
