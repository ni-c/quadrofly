/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	i2cmaster.c
 * @brief 	I2C master library using hardware TWI interface
 * @author  Peter Fleury <pfleury@gmx.ch>  http://jump.to/fleury
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 7, 2012
 */
#include "main.h"

#ifdef I2C_MASTER_AVAILABLE

#include "log.h"

#include <inttypes.h>
#include <compat/twi.h>

#include <i2cmaster.h>

#define SCL_CLOCK  	100000L /*!< I2C clock in Hz */

#endif /* I2C_MASTER_AVAILABLE */

/**
 * Initialization of the I2C bus interface. Need to be called only once
 */
void i2c_init(void) {
#ifdef I2C_MASTER_AVAILABLE
    /* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */

    TWSR = 0; /* no prescaler */
    TWBR = (uint8_t) ((F_CPU / SCL_CLOCK) - 16) / 2; /* must be > 10 for stable operation */
    log_s(" master ...");
#endif /* I2C_MASTER_AVAILABLE */
}/* i2c_init */

/**
 * Issues a start condition and sends address and transfer direction.
 * return 0 = device accessible, 1= failed to access device
 *
 * * @param address address and transfer direction of I2C device
 */
unsigned char i2c_start(unsigned char address) {
#ifdef I2C_MASTER_AVAILABLE
    uint8_t twst;

    // send START condition
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    // wait until transmission completed
    while (!(TWCR & (1 << TWINT)))
        ;

    // check value of TWI Status Register. Mask prescaler bits.
    twst = TW_STATUS & 0xF8;
    if ((twst != TW_START) && (twst != TW_REP_START))
        return 0;

    // send device address
    TWDR = address;
    TWCR = (1 << TWINT) | (1 << TWEN);

    // wail until transmission completed and ACK/NACK has been received
    while (!(TWCR & (1 << TWINT)))
        ;

    // check value of TWI Status Register. Mask prescaler bits.
    twst = TW_STATUS & 0xF8;
    if ((twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK))
        return 0;

#endif /* I2C_MASTER_AVAILABLE */
    return 1;

}/* i2c_start */

/**
 * Issues a start condition and sends address and transfer direction.
 * If device is busy, use ack polling to wait until device is ready
 *
 * @param address address and transfer direction of I2C device
 */
void i2c_start_wait(unsigned char address) {
#ifdef I2C_MASTER_AVAILABLE
    uint8_t twst;

    while (1) {
        // send START condition
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
        // wait until transmission completed
        while (!(TWCR & (1 << TWINT)))
            ;

        // check value of TWI Status Register. Mask prescaler bits.
        twst = TW_STATUS & 0xF8;
        if ((twst != TW_START) && (twst != TW_REP_START))
            continue;

        // send device address
        TWDR = address;
        TWCR = (1 << TWINT) | (1 << TWEN);

        // wail until transmission completed
        while (!(TWCR & (1 << TWINT)))
            ;

        // check value of TWI Status Register. Mask prescaler bits.
        twst = TW_STATUS & 0xF8;
        if ((twst == TW_MT_SLA_NACK) || (twst == TW_MR_DATA_NACK)) {
            /* device busy, send stop condition to terminate write operation */
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

            // wait until stop condition is executed and bus released
            while (TWCR & (1 << TWSTO))
                ;

            continue;
        }
        //if( twst != TW_MT_SLA_ACK) return 1;
        break;
    }
#endif /* I2C_MASTER_AVAILABLE */
}/* i2c_start_wait */

/**
 * Issues a repeated start condition and sends address and transfer direction
 *
 * @param address address and transfer direction of I2C device
 * @return 0 device accessible, 1 failed to access device
 */
unsigned char i2c_rep_start(unsigned char address) {
#ifdef I2C_MASTER_AVAILABLE
    return i2c_start(address);
#else
    return 0;
#endif /* I2C_MASTER_AVAILABLE */
}/* i2c_rep_start */

/**
 * Terminates the data transfer and releases the I2C bus
 */
void i2c_stop(void) {
#ifdef I2C_MASTER_AVAILABLE
    /* send stop condition */
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

    // wait until stop condition is executed and bus released
    while (TWCR & (1 << TWSTO))
        ;
#endif /* I2C_MASTER_AVAILABLE */
}/* i2c_stop */

/**
 * Send one byte to I2C device
 *
 * @param data byte to be transfered
 * @return 0 write successful, 1 write failed
 */
unsigned char i2c_write(unsigned char data) {
#ifdef I2C_MASTER_AVAILABLE
    uint8_t twst;

    // send data to the previously addressed device
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);

    // wait until transmission completed
    while (!(TWCR & (1 << TWINT)))
        ;

    // check value of TWI Status Register. Mask prescaler bits
    twst = TW_STATUS & 0xF8;
    if (twst != TW_MT_DATA_ACK)
        return 0;

#endif /* I2C_MASTER_AVAILABLE */
    return 1;
}/* i2c_write */

/**
 * Read one byte from the I2C device, request more data from device
 * @return byte read from I2C device
 */
unsigned char i2c_read_ack(void) {
#ifdef I2C_MASTER_AVAILABLE
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)))
        ;

    return TWDR;
#else
    return 0;
#endif /* I2C_MASTER_AVAILABLE */
}/* i2c_readAck */

/**
 * Read one byte from the I2C device, read is followed by a stop condition
 *
 * @return byte read from I2C device
 **/
unsigned char i2c_read_nak(void) {
#ifdef I2C_MASTER_AVAILABLE
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)))
        ;

    return TWDR;
#else
    return 0;
#endif /* I2C_MASTER_AVAILABLE */
}/* i2c_readNak */
