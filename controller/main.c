/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file 	controller/main.c
 * @brief 	Main program
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 6, 2012
 *
 * @defgroup controller Controller
 *
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
#include "pid.h"

#include <avr/io.h>
#include <avr/interrupt.h>

uint64_t lastlooptime = 0; /*!< Timestamp of the last loop */
uint8_t looptime = 0; /*!< Loop time in ms */

float kalman[3]; /*!< Kalman Filter results */
float pid[3]; /*!< PID controller results */

#ifdef MPU6050_AVAILABLE
int16_t mpu6050[7]; /*!< MPU-6050 measurements */
#endif /* MPU6050_AVAILABLE */

#ifdef MOTORCONTROL_AVAILABLE
int16_t motor[4] = { 0, 0, 0, 0 }; /*!< Calculated speed values */
uint8_t rc_channel[4] = { 0, 0, 0, 0 }; /*!< Values of the RC channels */
#endif /* MOTORCONTROL_AVAILABLE */

uint8_t speed[4] = { 0, 0, 0, 0 }; /*!< Speed values from the RC */

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
#endif /* MPU6050_AVAILABLE */
}

/**
 * Calculates the Kalman filter and PID controller values
 */
void pid_update(void) {
    for (int i = 0; i < 3; i++) {
        kalman[i] = kalman_calculate((float) mpu6050[i], (float) mpu6050[3 + i], looptime, i);
        pid[i] = pid_calculate(0, kalman[i]) / 10;
    }
}

/**
 * Calculates the motor speeds, sends them to the motorcontrol and reads the RC channels from
 * the motorcontrol.
 */
void motorcontrol_update(void) {
#ifdef MOTORCONTROL_AVAILABLE

    /* Calculate motor speeds */
    motor[0] = (speed[0] == 0) ? 0 : speed[0] + (pid[ACC_X] / 10);
    motor[1] = (speed[1] == 0) ? 0 : speed[1] - (pid[ACC_Y] / 10);
    motor[2] = (speed[2] == 0) ? 0 : speed[2] - (pid[ACC_X] / 10);
    motor[3] = (speed[3] == 0) ? 0 : speed[3] + (pid[ACC_Y] / 10);
    for (int i = 0; i < 4; ++i) {
        if (motor[i] > 255) {
            motor[i] = 255;
        } else if (motor[i] < 0) {
            motor[i] = 0;
        }
    }

    /* Set motor speed and read RC channel values */
    motorcontrol((uint8_t) motor[0], (uint8_t) motor[1], (uint8_t) motor[2], (uint8_t) motor[3], &rc_channel[0], &rc_channel[1], &rc_channel[2], &rc_channel[3]);

    /* Flatten RC channel values and prepare motor speed variables */
    uint8_t tmp_speed = (rc_channel[0] < 25) ? 0 : rc_channel[0];
    for (int i = 0; i < 4; ++i) {
        speed[i] = tmp_speed;
    }

#endif /* MOTORCONTROL_AVAILABLE */
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

        /* Kalman filter and PID controller for the MPU-6050 measurements */
        pid_update();

        /* Set motor speed and read RC channel values */
        motorcontrol_update();

    }

    /* Finally. (Never ever) */
    return 0;
}
