/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	motorcontrol/main.c
 * @brief 	Main program
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "global_def.h"
#include "init.h"
#include "motor.h"
#include "i2cslave.h"

#include <avr/delay.h>

/**
 * The main function.
 */
int main(void) {

    /* Initialization */
    init_qfly();

    /* Our loop */
    while (1) {

    }

    /* Finally. (Never ever) */
    return 0;
}
