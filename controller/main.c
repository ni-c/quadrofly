/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file 	controller/main.c
 * @brief 	Main program
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 */
#include "main.h"
#include "global_def.h"
#include "motorcontrol.h"
#include "init.h"
#include "uart.h"
#include "log.h"
#include "rfm12.h"
#include "i2cmaster.h"
#include "mpu6050.h"
#include "motorcom.h"
#include "millis.h"
#include "kalman.h"

#include <avr/io.h>
#include <avr/interrupt.h>

uint32_t lastlooptime = 0; /*!< Timestamp of the last loop */
uint8_t looptime = 0; /*!< Loop time in ms */

#ifdef MPU6050_AVAILABLE
int16_t mpu6050[7]; /*!< MPU-6050 measurements */
float kalman[3]; /*!< Kalman Filter results */
#endif /* MPU6050_AVAILABLE */

#ifdef MOTORCONTROL_AVAILABLE
uint8_t motor[4] = {0,0,0,0};
uint8_t rc_channel[4] = {0,0,0,0};
#endif MOTORCONTROL_AVAILABLE

#ifdef RFM12B_AVAILABLE
/**
 * Received a rf12b packet
 *
 * @param value The packet data
 */
void rfm12_receive(uint8_t value) {

}
#endif /* RFM12B_AVAILABLE */

/**
 * Update the MPU6050 values
 */
void mpu6050_update(void) {
#ifdef MPU6050_AVAILABLE
    /* Read MPU-6050 measurements */
    mpu6050_getall(&mpu6050[ACC_X], &mpu6050[ACC_Y], &mpu6050[ACC_Z], &mpu6050[GYRO_X], &mpu6050[GYRO_Y], &mpu6050[GYRO_Z], &mpu6050[TEMP]);

    /* Normalize MPU-6050 measurements*/
    mpu6050[ACC_X] = mpu6050[ACC_X] + ACC_X_OFFSET;
    mpu6050[ACC_Y] = mpu6050[ACC_Y] + ACC_Y_OFFSET;
    mpu6050[ACC_Z] = mpu6050[ACC_Z] + ACC_Z_OFFSET;

    /* Kalman filter the MPU-6050 measurements */
    for (int i = 0; i < 3; i++) {
        kalman[i] = kalmanCalculate((float)mpu6050[i], (float)mpu6050[3+i], looptime, i);
    }
#endif /* MPU6050_AVAILABLE */
}

/**
 * The main function.
 */
int main(void) {

    /* Initialization */
    init_qfly();
    log_s("up and running\n");

    /* Our loop */
    while (1) {

        /* Get time of the last program loop in milliseconds */
        looptime = millis() - lastlooptime;
        lastlooptime = millis();

        /* Update the MPU-6050 values */
        mpu6050_update();

        /* Set motor speed and read RC channel values */
        motorcontrol(motor[0], motor[1], motor[2], motor[3], &rc_channel[0], &rc_channel[1], &rc_channel[2], &rc_channel[3]);

#ifdef UART_AVAILABLE
        if (uart_tx_ready()) {
            /*
            uart_tx_int16_t(mpu6050[ACC_X]);
            uart_tx("\n");
            */
        }
#endif /* UART_AVAILABLE */
    }

    /* Finally. (Never ever) */
    return 0;
}
