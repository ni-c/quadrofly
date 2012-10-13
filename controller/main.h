/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	controller/main.h
 * @brief 	Global header file.
 * @ingroup controller
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#ifndef MAIN_H_
#define MAIN_H_

#ifndef F_CPU
#define F_CPU 10000000UL /*!< Set CPU clock */
#endif

#define I2C_BUFFER_SIZE 16 /*!< Size of the I2C buffer 2..254 */

#define LED_AVAILABLE /*!< LEDs available */

//#define LOG_AVAILABLE /*!< Is Logging available */

//#define UART_AVAILABLE /*!< Is UART available */

#define I2C_MASTER_AVAILABLE /*!< I2C is available as master */

#define RFM12B_AVAILABLE /*!< Is RFM12B available */

#define MPU6050_AVAILABLE /*!< Is MPU6050 available */

#define MOTORCONTROL_AVAILABLE /*!< Is the motor control module available */

#define EEPROM_AVAILABLE /*!< Is the EEPROM of the MCU available */

/* Dependencies */
#ifdef UART_AVAILABLE
#define BAUD 57600UL /*!< UART Baudrate in bit per second */
#endif

#ifdef MOTORCONTROL_AVAILABLE

#define I2C_MASTER_AVAILABLE /*!< We need I2C to communicate with the motor control */

#define RC_CAP 35 /*!< RC values lower than this cap should be ignored */

#define RC_SPEED 0 /*!< RC Speed channel */
#define RC_PITCH 1 /*!< RC Pitch channel */
#define RC_ROLL  2 /*!< RC Roll channel */
#define RC_YAW   3 /*!< RC Yaw channel */

#define RC_SENSITIVITY 2.0 /*!< Sensitivity of the RC channels for Pitch and Roll */

#endif /* MOTORCONTROL_AVAILABLE */

#ifdef MPU6050_AVAILABLE

#define I2C_MASTER_AVAILABLE /*!< We need I2C to communicate with MPU6050 */

/* mpu6050 buffer array positions */
#define ACC_X   0   /*!< X accelerator value */
#define ACC_Y   1   /*!< Y accelerator value */
#define ACC_Z   2   /*!< Z accelerator value */
#define TEMP    3   /*!< TEMP value */
#define GYRO_X  4   /*!< X gyroscope value */
#define GYRO_Y  5   /*!< Y gyroscope value */
#define GYRO_Z  6   /*!< Z gyroscope value */

#endif /* MPU6050_AVAILABLE */

#endif
