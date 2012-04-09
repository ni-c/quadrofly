/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	i2cslave.c
 * @brief 	I2C slave functions
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 19, 2012
 */
#include "main.h"
#include "i2cslave.h"
#include "inttypes.h"
#include "string.h"

#ifdef I2C_SLAVE_AVAILABLE

#include <util/twi.h>
#include <avr/interrupt.h>

struct {
    unsigned rx :1; /*<! If there is data in the RX buffer */
    unsigned tx :1; /*<! If there is data in the TX buffer */
}i2c_flag;

volatile uint8_t i2c_rx_buffer_addr; /*!< RX buffer address register */

volatile uint8_t i2c_tx_buffer_addr; /*!< TX buffer address register */

#define TWCR_ACK TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC); /*!< send ACK after receiving data / wait for ACK after sending data */

#define TWCR_NACK TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC); /*!< send NACK after receiving data / wait for ACK after sending data */

#define TWCR_RESET TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC); /*!< switch to the non adressed slave mode */

#endif /*I2C_SLAVE_AVAILABLE */

/**
 * Checks if there is new data in the I2C RX buffer
 *
 * @return 1 if there is new data in the I2C RX buffer
 */
uint8_t i2c_rx_ready(void) {
#ifdef I2C_SLAVE_AVAILABLE
    if (i2c_flag.rx==1) {
        i2c_flag.rx = 0;
        return 1;
    }
#endif /*I2C_SLAVE_AVAILABLE */
    return 0;
}

/**
 * Initialize the device as I2C slave with the given address
 *
 * @param addr The address of the I2C slave
 */
void i2c_slave_init(uint8_t addr) {
#ifdef I2C_SLAVE_AVAILABLE
    TWAR = addr;
    TWCR &= ~(1 << TWSTA) | (1 << TWSTO);
    TWCR |= (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
    i2c_rx_buffer_addr = 0xFF;
    i2c_tx_buffer_addr = 0xFF;
#endif /* I2C_SLAVE_AVAILABLE */
}

#ifdef I2C_SLAVE_AVAILABLE
/**
 * TWI ISR
 */
ISR (TWI_vect) {
    uint8_t data = 0;
    switch (TW_STATUS) {
// Slave Receiver
        case TW_SR_SLA_ACK:// 0x60 slave receiver, slave is addressed
        i2c_rx_buffer_addr = 0xFF;// bufferposition is undefined
        PORTB = (1 << PB4);
        TWCR_ACK// receive next byte, send ACK
        break;

        case TW_SR_DATA_ACK:// 0x80 slave receiver, received a byte
        data = TWDR;// read byte
        if (i2c_rx_buffer_addr == 0xFF) {
            if (data < I2C_BUFFER_SIZE) {
                i2c_rx_buffer_addr = data;
            } else {
                i2c_rx_buffer_addr = 0;
            }
        } else {
            if (i2c_rx_buffer_addr < I2C_BUFFER_SIZE) {
                i2c_rx_buffer[i2c_rx_buffer_addr] = data;  // write data to buffer
            }
            i2c_rx_buffer_addr++;
        }
        TWCR_ACK
        break;

//Slave transmitter
        case TW_ST_SLA_ACK://0xA8 slave is addressed for read and answered ACK
        case TW_ST_DATA_ACK://0xB8 slave transmitter, data requested

        if (i2c_tx_buffer_addr == 0xFF) {
            i2c_tx_buffer_addr = 0;
        }
        if (i2c_tx_buffer_addr < I2C_BUFFER_SIZE) {
            TWDR = i2c_tx_buffer[i2c_tx_buffer_addr];  // send byte
            i2c_tx_buffer_addr++;// bufferaddress for next byte
        } else {
            TWDR = 0;  // no more data
            i2c_flag.tx = 0;
        }
        TWCR_ACK
        break;

        case TW_ST_DATA_NACK:  // 0xC0 no more data
        case TW_SR_DATA_NACK:// 0x88
        case TW_ST_LAST_DATA:// 0xC8 last data byte in TWDR has been transmitted (TWEA = “0”); ACK has been received
        case TW_SR_STOP:// 0xA0 received STOP
        default:
        i2c_flag.rx = 1;
        TWCR_RESET
        break;

    }
}
#endif /* I2C_SLAVE_AVAILABLE */
