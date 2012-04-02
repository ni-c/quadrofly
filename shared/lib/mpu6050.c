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

#include <inttypes.h>

/**
 * Initializes the MPU-6050 device
 */
void mpu6050_init(void) {
#ifdef MPU6050_AVAILABLE
    log_s("mpu6050 initialization ...");
    if (i2c_start(MPU6050_ADDRESS + I2C_WRITE)) {

        /* Set clock to X GYRO and disable sleep */
        i2c_write(0x6B);
        i2c_write(0x03);

        /* Sampe rate = 200 Hz Fsample= 1Khz/(4+1) = 200Hz */
        i2c_write(0x19);
        i2c_write(0x04);

        /* Low pass filter */
        i2c_write(0x1A);
        i2c_write(0x03);

        /* Selects the full scale range of the gyroscope outputs to ± 250 °/s */
        i2c_write(0x1B);
        i2c_write(0x00);

        /* Selects the full scale range of the accelerometer outputs to 2g and sets the High Pass filter to 2.5 Hz */
        i2c_write(0x1C);
        i2c_write(0x0A);

        log_s(" ok\n");
    } else {
        log_s(" failed\n");
    }
    i2c_stop();
#endif // MPU6050_AVAILABLE
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
 * Returns a 16 bit value from the mpu6050
 *
 * @param reg_address The address of the first register to read the value from
 * @result The 16-bit value of the register
 */
uint16_t mpu6050_get(uint8_t reg_address) {
#ifdef MPU6050_AVAILABLE
    i2c_start(MPU6050_ADDRESS + I2C_WRITE);
    i2c_write(reg_address);
    i2c_rep_start(MPU6050_ADDRESS + I2C_READ);
    uint8_t hi = i2c_read_nak();
    i2c_rep_start(MPU6050_ADDRESS + I2C_WRITE);
    i2c_write(reg_address + 1);
    i2c_rep_start(MPU6050_ADDRESS + I2C_READ);
    uint8_t lo = i2c_read_nak();
    i2c_stop();
    uint16_t result = (((int16_t) hi) << 8) | lo;
    return result;
#else // MPU6050_AVAILABLE
    return 0;
#endif // MPU6050_AVAILABLE
}
