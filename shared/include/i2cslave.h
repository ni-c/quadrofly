/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	i2cslave.h
 * @brief   I2C slave definitions
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 19, 2012
 */

#ifndef I2CSLAVE_H_
#define I2CSLAVE_H_

#include <inttypes.h>

#define I2C_BUFFER_SIZE 8 /*!< Size of the I2C buffer 2..254 */

volatile uint8_t i2c_rx_buffer[I2C_BUFFER_SIZE + 1]; /*!< The i2c RX buffer */

volatile uint8_t i2c_tx_buffer[I2C_BUFFER_SIZE + 1]; /*!< The i2c TX buffer */

/**
 * Checks if there is new data in the I2C RX buffer
 *
 * @return 1 if there is new data in the I2C RX buffer
 */
uint8_t i2c_rx_ready(void);

#endif /* I2CSLAVE_H_ */
