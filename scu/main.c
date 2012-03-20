/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	receiver/main.c
 * @brief 	Main program
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "global_def.h"
#include "init.h"
#include "log.h"
#include "i2cmaster.h"
#include "rfm12.h"

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

/**
 * Received a rf12b packet
 *
 * @param value The packet data
 */
void rfm12_receive(uint8_t value) {
	if (!(i2c_start(RECEIVER_I2C_ADDR + I2C_WRITE))) //slave ready to write?
	{
		i2c_write(0x00); // set buffer start address
		i2c_write(value);
		i2c_stop();
	}
}


/**
 * The main function.
 */
int main(void) {

	DDRC = 0xff; // set port c to output (there are our testing leds)
	DDRD &= ~( (1<<PD2) | (1<<PD3) );  // set ports PD2 and PD3 as input

	/* Initialization */
	init_qfly();
	log_s("initialization ... ok\n");

	/* Our loop */
	while (1) {

		/* Wait 500ms */
		_delay_ms(100);
		PORTC |= (1 << PC3);  // enable LED 1

		/* Wait 500ms */
		_delay_ms(100);
		PORTC &= ~(1 << PC3); // disable LED 1
	}

	/* Finally. (Never ever) */
	return 0;
}
