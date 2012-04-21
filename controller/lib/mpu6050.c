/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	mpu6050.c
 * @brief   MPU6050 I2C device class.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 1, 2012
 */

#include "main.h"
#include "mpu6050.h"
#include "i2cmaster.h"
#include "log.h"
#include "uart.h"

#include <inttypes.h>

/**
 * Initializes the MPU-6050 device
 */
uint8_t mpu6050_init(void) {
#ifdef MPU6050_AVAILABLE
    log_s("mpu6050 ...");

    /* Set clock to X GYRO and disable sleep */
    mpu6050_set(MPU6050_PWR_MGMT_1, 0x01);

    /* Set the low pass filter to 2 */
    mpu6050_set(MPU6050_CONFIG, 0x02);

    /* Set the scale range of the gyroscope to ± 2000 °/s */
    mpu6050_set(MPU6050_GYRO_CONFIG, 0x1B);

    /* Set the scale range of the acclerometer to ± 16g and the high pass filter to 5Hz */
    mpu6050_set(MPU6050_ACCEL_CONFIG, 0x19);

    /* Enable DATA_RDY interrupt */
    mpu6050_set(MPU6050_INT_ENABLE, 0x01);

    /* Set interrupt pin D3 to input */DDRD &= ~(1 << DD3);

    if (mpu6050_test()) {
        log_s(" ok\n");
        return 1;
    } else {
        log_s(" failed\n");
        return 0;
    }
#endif // MPU6050_AVAILABLE
}

/**
 * Checks if the data ready interrupt of the MPU-6050 is set
 *
 * @return 1 if the data ready interrupt of the MPU-6050 is set
 */
uint8_t mpu6050_data_ready(void) {
    if ( PIND & PD3 ) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Set a register in the MPU-6050
 *
 * @param regaddress The register address to set
 * @param value The value to set the register to
 */
uint8_t mpu6050_set(uint8_t regaddress, uint8_t value) {
#ifdef MPU6050_AVAILABLE
    if (i2c_start(MPU6050_ADDRESS + I2C_WRITE)) {
        i2c_write(0x1C);
        i2c_write(0x19);
        i2c_stop();
        return 1;
    }
#endif // MPU6050_AVAILABLE
    return 0;
}

/**
 * Test the I2C communication with the mpu6050 module.
 *
 * @return 1 If the communication is working
 */
uint8_t mpu6050_test(void) {
#ifdef MPU6050_AVAILABLE
    if (i2c_start(MPU6050_ADDRESS + I2C_WRITE)) {
        i2c_write(0x75);
        if (i2c_rep_start(MPU6050_ADDRESS + I2C_READ)) {
            uint8_t addr = i2c_read_nak();
            i2c_stop();
            return (addr == 0x68);
        }
    }
    i2c_stop();
#endif // MPU6050_AVAILABLE
    return 0;
}

/**
 * Returns a SIGNED 16 bit value from the mpu6050
 *
 * @param reg_address The address of the first register to read the value from
 * @result The SIGNED 16-bit value of the register
 */
int16_t mpu6050_get(uint8_t reg_address) {
#ifdef MPU6050_AVAILABLE
    if (i2c_start(MPU6050_ADDRESS + I2C_WRITE)) {
        i2c_write(reg_address);
        if (i2c_rep_start(MPU6050_ADDRESS + I2C_READ)) {
            int16_t result = (((int16_t) i2c_read_ack()) << 8) | i2c_read_nak();
            i2c_stop();
            return result;
        }
    }
#endif // MPU6050_AVAILABLE
    return 0;
}

/**
 * Returns an UNSIGNED 16 bit value from the mpu6050
 *
 * @param reg_address The address of the first register to read the value from
 * @result The UNSIGNED 16-bit value of the register
 */
uint16_t mpu6050_u_get(uint8_t reg_address) {
#ifdef MPU6050_AVAILABLE
    if (i2c_start(MPU6050_ADDRESS + I2C_WRITE)) {
        i2c_write(reg_address);
        if (i2c_rep_start(MPU6050_ADDRESS + I2C_READ)) {
            uint16_t result = (((uint16_t) i2c_read_ack()) << 8) | i2c_read_nak();
            i2c_stop();
            return result;
        }
    }
#endif // MPU6050_AVAILABLE
    return 0;
}

/**
 * Reads the interrupt status of the MPU-6050 and returns it
 *
 * @result The interrupt status of the MPU-6050 register
 */
uint8_t mpu6050_get_int_status(void) {
#ifdef MPU6050_AVAILABLE
    if (i2c_start(MPU6050_ADDRESS + I2C_WRITE)) {
        i2c_write(MPU6050_INT_STATUS);
        if (i2c_rep_start(MPU6050_ADDRESS + I2C_READ)) {
            uint8_t result = i2c_read_ack();
            i2c_stop();
            return result;
        }
    }
#endif // MPU6050_AVAILABLE
    return 0;
}

/**
 * Reads the MPU-6050 registers with the most recent accelerometer, temperature sensor and gyroscope measurements
 *
 * @param accel_xout Pointer to store the most recent X accelerometer measurement
 * @param accel_yout Pointer to store the most recent Y accelerometer measurement
 * @param accel_zout Pointer to store the most recent Z accelerometer measurement
 * @param gyro_xout Pointer to store the most recent X gyroscope measurement
 * @param gyro_yout Pointer to store the most recent Y gyroscope measurement
 * @param gyro_zout Pointer to store the most recent Z gyroscope measurement
 * @param temp_out Pointer to store the most recent temperature sensor measurement
 */
void mpu6050_getall(int16_t *accel_xout, int16_t *accel_yout, int16_t *accel_zout, int16_t *gyro_xout, int16_t *gyro_yout, int16_t *gyro_zout, int16_t *temp_out) {
#ifdef MPU6050_AVAILABLE
    if (i2c_start(MPU6050_ADDRESS + I2C_WRITE)) {
        i2c_write(MPU6050_ACCEL_XOUT);
        if (i2c_rep_start(MPU6050_ADDRESS + I2C_READ)) {
            *accel_xout = (((int16_t) i2c_read_ack()) << 8) | i2c_read_ack();
            *accel_yout = (((int16_t) i2c_read_ack()) << 8) | i2c_read_ack();
            *accel_zout = (((int16_t) i2c_read_ack()) << 8) | i2c_read_ack();
            *temp_out = (((int16_t) i2c_read_ack()) << 8) | i2c_read_ack();
            *gyro_xout = (((int16_t) i2c_read_ack()) << 8) | i2c_read_ack();
            *gyro_yout = (((int16_t) i2c_read_ack()) << 8) | i2c_read_ack();
            *gyro_zout = (((int16_t) i2c_read_ack()) << 8) | i2c_read_nak();
        }
    }
    i2c_stop();

    // Clear interrupt register
    mpu6050_get_int_status();
#endif // MPU6050_AVAILABLE
}
