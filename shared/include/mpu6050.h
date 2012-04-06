/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	mpu6050.h
 * @brief   MPU6050 I2C device class.
 * @author  Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 1, 2012
 */

#include <inttypes.h>

#ifndef MPU6050_H_
#define MPU6050_H_

#define MPU6050_ADDRESS_AD0_LOW     0xD0 /*!< address pin low (GND) */
#define MPU6050_ADDRESS_AD0_HIGH    0xD2 /*!< address pin high (VCC) */
#define MPU6050_ADDRESS             MPU6050_ADDRESS_AD0_LOW /*!< MPU6050 address */

#define MPU6050_ACCEL_XOUT          0x3B /*!< hi byte of the ACCEL_XOUT register */
#define MPU6050_ACCEL_YOUT          0x3D /*!< hi byte of the ACCEL_YOUT register */
#define MPU6050_ACCEL_ZOUT          0x3F /*!< hi byte of the ACCEL_ZOUT register */
#define MPU6050_TEMP_OUT            0x41 /*!< hi byte of the TEMP_OUT register */
#define MPU6050_GYRO_XOUT           0x43 /*!< hi byte of the GYRO_XOUT register */
#define MPU6050_GYRO_YOUT           0x45 /*!< hi byte of the GYRO_YOUT register */
#define MPU6050_GYRO_ZOUT           0x47 /*!< hi byte of the GYRO_ZOUT register */

#define MPU6050_PWR_MGMT_1          0x6B /*!< This register allows the user to configure the power mode and clock source. It also provides a bit for resetting the entire device, and a bit for disabling the temperature sensor. */
#define MPU6050_PWR_MGMT_2          0x6C /*!< This register allows the user to configure the frequency of wake-ups in Accelerometer Only Low Power Mode. This register also allows the user to put individual axes of the accelerometer and gyroscope into standby mode. */
#define MPU6050_CONFIG              0x1A /*!< This register configures the external Frame Synchronization (FSYNC) pin sampling and the Digital Low Pass Filter (DLPF) setting for both the gyroscopes and accelerometers. */
#define MPU6050_GYRO_CONFIG         0x1B /*!< This register is used to trigger gyroscope self-test and configure the gyroscopes’ full scale range. */
#define MPU6050_ACCEL_CONFIG        0x1C /*!< This register is used to trigger accelerometer self test and configure the accelerometer full scale range. This register also configures the Digital High Pass Filter (DHPF). */

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
 * Set a register in the MPU-6050
 *
 * @param regaddress The register address to set
 * @param value The value to set the register to
 */
uint8_t mpu6050_set(uint8_t regaddress, uint8_t value);

/**
 * Returns a SIGNED 16 bit value from the mpu6050
 *
 * @param reg_address The address of the first register to read the value from
 * @result The SIGNED 16-bit value of the register
 */
int16_t mpu6050_get(uint8_t reg_address);

/**
 * Returns an UNSIGNED 16 bit value from the mpu6050
 *
 * @param reg_address The address of the first register to read the value from
 * @result The UNSIGNED 16-bit value of the register
 */
uint16_t mpu6050_u_get(uint8_t reg_address);

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
void mpu6050_getall(int16_t *accel_xout, int16_t *accel_yout, int16_t *accel_zout, int16_t *gyro_xout, int16_t *gyro_yout, int16_t *gyro_zout, int16_t *temp_out);

#endif /* MPU6050_H_ */
