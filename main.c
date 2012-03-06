/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	main.c
 * @brief 	Main program
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "init.h"

#include <avr/io.h>

/**
 * The main function.
 */
int main(int argc, char * argv[]) {

	init_qfly(argc, argv);

	/* Set port PA0 to output */
	DDRA = (1 << DDA0);

	/* Pull port PA0 up */
	PORTA = (1 << PA0);

	while (1) {
		/* Our loop */
	}

	/* Finally. (Never ever) */
	return 0;
}
