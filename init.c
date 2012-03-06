/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	init.c
 * @brief 	Global initialization file.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "uart.h"

/**
 * Quadrofly initialization
 *
 * @param argc argument count
 * @param argv arguments
 */
void init_qfly(int argc, char * argv[]) {

#ifdef UART_AVAILABLE
	/*
	 * Initialize UART
	 */
	uart_init();
#endif

}
