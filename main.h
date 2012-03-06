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

/**
 * CPU Speed
 */
#ifndef F_CPU
#define F_CPU 20000000UL
#endif

/**
 * Debugging
 */
//#define SIMULAVR_AVAILABLE

/**
 * UART
 */
#define UART_AVAILABLE
#define BAUD 57600UL

#endif
