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

float kalman[3] = { 0, 0, 0 }; /*!< Kalman Filter results */
int16_t pid[4] = { 0, 0, 0, 0 }; /*!< Calculated speed values */

#ifdef MPU6050_AVAILABLE
int16_t mpu6050[7] = { -1 * ACC_X_OFFSET, -1 * ACC_Y_OFFSET, -1 * ACC_Z_OFFSET, 0, 0, 0, 0 }; /*!< MPU-6050 measurements */
#endif /* MPU6050_AVAILABLE */

#ifdef MOTORCONTROL_AVAILABLE
uint8_t rc_channel[4] = { 0, 0, 0, 0 }; /*!< Values of the RC channels */
#endif /* MOTORCONTROL_AVAILABLE */

uint8_t speed[4] = { 0, 0, 0, 0 }; /*!< Speed values from the RC */

int16_t target[4] = { 0, 0, 0, 0 };

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

#ifdef LOG_AVAILABLE
    log_s("MPU");
    for (int i = 0; i < 7; ++i) {
        log_s(";");
        log_int16_t((int16_t)mpu6050[i]);
    }
    log_s("\n");
#endif /* LOG_AVAILABLE */

#endif /* MPU6050_AVAILABLE */
}

/**
 * Calculates the Kalman filter and PID controller values
 */
void pid_update(void) {

    /* Calculate kalman values */
    for (int i = 0; i < 3; i++) {
        kalman[i] = 1 + kalman_calculate((float) mpu6050[i], (float) mpu6050[3 + i], looptime, i) / 24000;
    }

    /* Calculate target speeds */
    target[0] = (int16_t) ((float) speed[0] * kalman[ACC_X] * (2 - kalman[ACC_Y]));
    target[1] = (int16_t) ((float) speed[1] * (2 - kalman[ACC_X]) * kalman[ACC_Y]);
    target[2] = (int16_t) ((float) speed[2] * (2 - (kalman[ACC_X] * kalman[ACC_Y])));
    target[3] = (int16_t) ((float) speed[3] * kalman[ACC_X] * kalman[ACC_Y]);

    /* PID control */
    for (int i = 0; i < 4; ++i) {
        pid[i] = (target[i] == 0) ? 0 : pid_calculate(target[i], pid[i], i);
        if (pid[i] > 255) {
            pid[i] = 255;
        } else if (pid[i] < 0) {
            pid[i] = 0;
        }
    }
}

/**
 * Calculates the motor speeds, sends them to the motorcontrol and reads the RC channels from
 * the motorcontrol.
 */
void motorcontrol_update(void) {
#ifdef MOTORCONTROL_AVAILABLE

    /**
     * Flattens the given value
     * @param value The value to flatten
     * @param cap The cap under which the value should be flattened
     * @result The flatten value
     */
    uint8_t flatten(int16_t value, uint8_t cap) {
        return (value < cap) ? 0 : value;
    }

#ifdef LOG_AVAILABLE
    log_s("PID");
    for (int i = 0; i < 4; ++i) {
        log_s(";");
        log_uint8_t(pid[i]);
    }
    log_s("\n");
#endif /* LOG_AVAILABLE */

    /* Set motor speed and read RC channel values */
    motorcontrol((uint8_t) pid[0], (uint8_t) pid[1], (uint8_t) pid[2], (uint8_t) pid[3], &rc_channel[0], &rc_channel[1], &rc_channel[2], &rc_channel[3]);

#ifdef LOG_AVAILABLE
    log_s("RC");
    for (int i = 0; i < 4; ++i) {
        log_s(";");
        log_uint8_t(rc_channel[i]);
    }
    log_s("\n");
#endif /* LOG_AVAILABLE */

    /* Flatten RC channel speed value */
    rc_channel[0] = (flatten((int16_t) rc_channel[0], 25) / 2);

    /* Calculate motor speed values */
    for (int i = 0; i < 4; ++i) {
        speed[i] = rc_channel[0];
    }

    /* Add pitch and roll direction */
    if ((speed[0] > 0) && (speed[1] > 0) && (speed[2] > 0) && (speed[3] > 0)) {

        int16_t tmp;
        int16_t pitch = (((int16_t) (rc_channel[2] / 2)) - 50);
        int16_t roll = (((int16_t) (rc_channel[1] / 2)) - 50);

        if (pitch >= 0) {
            tmp = speed[0] + pitch;
            speed[0] = (tmp > 250) ? 250 : tmp;
        } else {
            tmp = speed[1] + (-1 * pitch);
            speed[1] = (tmp > 250) ? 250 : tmp;
        }

        if (roll >= 0) {
            tmp = speed[2] + roll;
            speed[2] = (tmp > 250) ? 250 : tmp;
        } else {
            tmp = speed[3] + (-1 * roll);
            speed[3] = (tmp > 250) ? 250 : tmp;
        }
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
