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

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

#ifdef SNAP_AVAILABLE
prog_char e_test[] = {0xA0, 0x00, 0x01, 0x20, 0x37, 0x32, 0x78, 0x49};

/**
 * Callback method that is called, if a snap packet is received
 *
 * @param status The status of the snap packet
 * @param *buf The buffer of the snap packet
 */
void snap_receive(uint8_t status, uint8_t *buf) {
	// do whatever you want, maybe...
	if (snap_fail(status))
	return;
	if (!snap_is_for_me())
	return;
}
#endif

/**
 * The main function.
 */
int main(void) {

	DDRC = 0xff; // set port c to output (there are our testing leds)

	/* Initialization */
	init_qfly();
	log_s("initialization ... ok\n");

	/*
	 * Initialize SNAP
	 */
#ifdef SNAP_AVAILABLE
	snap_init(&snap_receive);
#endif /* SNAP_AVAILABLE */

	/* Our loop */
	while (1) {

		/* Wait 500ms */
		_delay_ms(500);
		PORTC |= (1 << PC5);  // enable LED 1

		/* Wait 500ms */
		_delay_ms(500);
		PORTC &= ~(1 << PC5); // disable LED 1

#ifdef I2C_MASTER_AVAILABLE
	if (!(i2c_start(RECEIVER_I2C_ADDR + I2C_WRITE))) //slave ready to write?
	{
		i2c_write(0x00); // set buffer start address
		i2c_write(0x11);
		i2c_write(0x12);
		i2c_write(0x13);
		i2c_stop(); // finish
	}
#endif /* I2C_MASTER_AVAILABLE */

#ifdef SNAP_AVAILABLE
		snap_send(e_test, SNAP_SEND_EDM_CRC_8 | SNAP_SEND_LEN_8 | SNAP_SEND_MEM_FLASH, 0x12);
#endif /* SNAP_AVAILABLE */
	}

	/* Finally. (Never ever) */
	return 0;
}
