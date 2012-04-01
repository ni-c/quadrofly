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

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

/**
 * The main function.
 */
int main(void) {

    /* Initialization */
    init_qfly();
    log_s("initialization ... ok\n");

    /* Our loop */
    while (1) {
    }

    /* Finally. (Never ever) */
    return 0;
}
