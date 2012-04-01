/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	mpu6050.c
 * @brief   MPU6050 I2C device class.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 1, 2012
 */

#include "mpu6050.h"
#include "i2cmaster.h"

/**
 * Initializes the MPU-6050 device
 */
void mpu6050_init(void) {
    i2c_start_wait(MPU6050_ADDRESS + I2C_WRITE);

    /* Set clock to X GYRO and disable sleep */
    i2c_write(0x6B);
    i2c_write(0x01);

    /* Selects the full scale range of the gyroscope outputs to ± 250 °/s */
    i2c_write(0x1B);
    i2c_write(0x00);


    /* Selects the full scale range of the accelerometer outputs to 2g and sets the High Pass filter to 2.5 Hz */
    i2c_write(0x1C);
    i2c_write(0x0A);

    i2c_stop();
}

/**
 * Test the I2C communication with the mpu6050 module.
 *
 * @return 1 If the communication is working
 */
uint8_t mpu6050_test(void) {

    i2c_start_wait(MPU6050_ADDRESS + I2C_WRITE);
    i2c_write(0x75);
    i2c_rep_start(MPU6050_ADDRESS + I2C_READ);
    uint8_t addr = i2c_readNak();
    i2c_stop();
    return (addr == MPU6050_ADDRESS);
}

/**
 * Returns a 16 bit value from the mpu6050
 *
 * @param reg_address The address of the first register to read the value from
 * @result The 16-bit value of the register
 */
uint16_t mpu6050_get(uint8_t reg_address) {
    i2c_start_wait(MPU6050_ADDRESS + I2C_WRITE);
    i2c_write(reg_address);
    i2c_rep_start(MPU6050_ADDRESS + I2C_READ);
    uint8_t hi = i2c_readNak();
    i2c_rep_start(MPU6050_ADDRESS + I2C_WRITE);
    i2c_write(reg_address+1);
    i2c_rep_start(MPU6050_ADDRESS + I2C_READ);
    uint8_t lo = i2c_readNak();
    i2c_stop();
    uint16_t result = (((int16_t)hi) << 8) | lo;
    return result;
}
