/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	controller/main.h
 * @brief 	Global header file.
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#ifndef MAIN_H_
#define MAIN_H_

#ifndef F_CPU
#define F_CPU 10000000UL /*!< Set CPU clock */
#endif

#define LED_AVAILABLE /*!< LEDs available */

//#define SIMULAVR_AVAILABLE /*!< Compile for the simulavr simulator */
#define LOG_AVAILABLE /*!< Is Logging available */

#define UART_AVAILABLE /*!< Is UART available */
#define BAUD 57600UL /*!< UART Baudrate in bit per second */

//#define I2C_MASTER_AVAILABLE /*!< I2C is available as master */

#define RFM12B_AVAILABLE /*!< Is RFM12B available */

#define MPU6050_AVAILABLE /*!< Is MPU6050 available */

#define MOTORCONTROL_AVAILABLE /*!< Is the motor control module available */

/* Dependencies */
#ifdef SIMULAVR_AVAILABLE
#undef UART_AVAILABLE /*!< UART doesn't work if the software runs on simulavr, deactivate it */
#endif /* SIMULAVR_AVAILABLE */

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
#endif


#endif
