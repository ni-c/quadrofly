/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	mpu6050.h
 * @brief   MPU6050 I2C device class.
 * @author  Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 1, 2012
 */

#ifndef MPU6050_H_
#define MPU6050_H_

#define MPU6050_ADDRESS_AD0_LOW     0x68 /*!< address pin low (GND) */
#define MPU6050_ADDRESS_AD0_HIGH    0x69 /*!< address pin high (VCC) */
#define MPU6050_ADDRESS             MPU6050_ADDRESS_AD0_LOW /*!< MPU6050 address */

#define MPU6050_ACCEL_XOUT          0x3B /*!< lo byte of the ACCEL_XOUT register */
#define MPU6050_ACCEL_YOUT          0x3D /*!< lo byte of the ACCEL_YOUT register */
#define MPU6050_ACCEL_ZOUT          0x3F /*!< lo byte of the ACCEL_ZOUT register */
#define MPU6050_TEMP_OUT            0x41 /*!< lo byte of the TEMP_OUT register */
#define MPU6050_GYRO_XOUT           0x43 /*!< lo byte of the GYRO_XOUT register */
#define MPU6050_GYRO_YOUT           0x45 /*!< lo byte of the GYRO_YOUT register */
#define MPU6050_GYRO_ZOUT           0x47 /*!< lo byte of the GYRO_ZOUT register */

/**
 * Initializes the MPU-6050 device
 */
void mpu6050_init(void);

/**
 * Test the I2C communication with the mpu6050 module.
 *
 * @return 1 If the communication is working
 */
uint8_t mpu6050_test(void);

/**
 * Returns a 16 bit value from the mpu6050
 *
 * @param reg_address The address of the first register to read the value from
 * @result The 16-bit value of the register
 */
uint16_t mpu6050_get(uint8_t reg_address);

#endif /* MPU6050_H_ */
