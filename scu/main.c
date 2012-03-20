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
#include "snap.h"
#include "rfm12.h"

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

#ifdef SNAP_AVAILABLE

void dbg(uint8_t value) {
	if (!(i2c_start(RECEIVER_I2C_ADDR + I2C_WRITE))) //slave ready to write?
	{
		i2c_write(0x00);
		i2c_write(value);
	}
	i2c_stop();
}

/**
 * Callback method that is called, if a snap packet is received
 *
 * @param status The status of the snap packet
 * @param *buf The buffer of the snap packet
 */
void snap_receive(uint8_t status, uint8_t *buf) {
	// do whatever you want, maybe...
//	if (snap_fail(status))
//	return;
//	if (!snap_is_for_me())
//	return;

#ifdef I2C_MASTER_AVAILABLE
	PORTC |= (1 << PC2);  // enable LED 2

	if (!(i2c_start(RECEIVER_I2C_ADDR + I2C_WRITE))) //slave ready to write?
	{
		i2c_write(0x00); // set buffer start address
		i2c_write(buf[0]);
		i2c_write(buf[1]);
		i2c_write(buf[2]);
		i2c_write(buf[3]);
		i2c_write(buf[4]);
		i2c_write(buf[5]);
		i2c_write(buf[6]);
		i2c_write(buf[7]);
		i2c_stop(); // finish
	}
#endif /* I2C_MASTER_AVAILABLE */

}
#endif

/**
 * The main function.
 */
int main(void) {

	DDRC = 0xff; // set port c to output (there are our testing leds)
	DDRD &= ~( (1<<PD2) | (1<<PD3) );  // set ports PD2 and PD3 as input

	/* Initialization */
	init_qfly();
	log_s("initialization ... ok\n");
	/*
	 * Initialize SNAP
	 */
#ifdef SNAP_AVAILABLE
	snap_init(&snap_receive);
#endif /* SNAP_AVAILABLE */

	uint16_t rx = rfm12_write(0x0000);
	if (!(i2c_start(RECEIVER_I2C_ADDR + I2C_WRITE))) //slave ready to write?
	{
		i2c_write(0x00); // set buffer start address
		i2c_write(0xff);
		i2c_write(rx & 0x00ff);
		i2c_write(rx & 0xff00);
		i2c_write(0xff);
		i2c_stop();
	}

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
