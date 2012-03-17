/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	receiver/main.h
 * @brief 	Global header file.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#ifndef MAIN_H_
#define MAIN_H_

#ifndef F_CPU
#define F_CPU 10000000UL /*!< Set CPU clock */
#endif

//#define SIMULAVR_AVAILABLE /*!< Compile for the simulavr simulator */

#define UART_AVAILABLE /*!< Is UART available */
#define BAUD 57600UL /*!< UART Baudrate in bit per second */

#define RFM12B_AVAILABLE /*!< Is RFM12B available */
#define SNAP_AVAILABLE /*!< Is Snap protocol available */

/* Dependencies */
#ifdef SIMULAVR_AVAILABLE
#undef UART_AVAILABLE /*!< UART doesn't work if the software runs on simulavr, deactivate it */
#endif /* SIMULAVR_AVAILABLE */

#endif
