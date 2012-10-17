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
#include <avr/delay.h>
#include <avr/wdt.h>

uint64_t lastlooptime = 0; /*!< Timestamp of the last loop */
uint8_t looptime = 0; /*!< Loop time in ms */

float kalman[3] = { 0, 0, 0 }; /*!< Kalman Filter results */
float pid[4] = { 0, 0, 0, 0 }; /*!< Calculated speed values */

#ifdef MPU6050_AVAILABLE
int16_t mpu6050[7] = { -ACC_X_OFFSET, -ACC_Y_OFFSET, -ACC_Z_OFFSET, 0, 0, 0, 0 }; /*!< MPU-6050 measurements */
#endif /* MPU6050_AVAILABLE */

#ifdef MOTORCONTROL_AVAILABLE
uint8_t rc_channel[4] = { 0, 0, 0, 0 }; /*!< Values of the RC channels */
#endif /* MOTORCONTROL_AVAILABLE */

uint8_t motor[4] = { 0, 0, 0, 0 }; /*!< Speed values from the RC */

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
 * Parse float into uint8_t
 *
 * @param f The float to parse
 * @return The parsed float as uint8_t
 */
uint8_t parseFloat(float f) {
    if (f < 0.0) {
        return 0;
    }
    return (f > 255.0) ? 255 : (uint8_t) f;
}

/**
 * The main function.
 */
int main(void) {

    /* Initialization */
    init_qfly();
    log_s("up and running\n");

    /* Enable watchdog */
    wdt_enable(WDTO_125MS);

    /* Our loop */
    while (1) {

        /* Get time of the last program loop in milliseconds */
        looptime = millis() - lastlooptime;
        lastlooptime = millis();

#ifdef MPU6050_AVAILABLE
        /* Read MPU-6050 measurements */
        mpu6050_getall(&mpu6050[ACC_X], &mpu6050[ACC_Y], &mpu6050[ACC_Z], &mpu6050[GYRO_X], &mpu6050[GYRO_Y], &mpu6050[GYRO_Z], &mpu6050[TEMP]);

        /* Normalize MPU-6050 measurements*/
        mpu6050[ACC_X] = mpu6050[ACC_X] + ACC_X_OFFSET;
        mpu6050[ACC_Y] = mpu6050[ACC_Y] + ACC_Y_OFFSET;
        mpu6050[ACC_Z] = mpu6050[ACC_Z] + ACC_Z_OFFSET;
#endif /* MPU6050_AVAILABLE */

#ifdef MOTORCONTROL_AVAILABLE
        /* Set motor speed and read RC channel values */
        motorcontrol(motor[0], motor[1], motor[2], motor[3], &rc_channel[0], &rc_channel[1], &rc_channel[2], &rc_channel[3]);

        /* Flatten RC channel speed value */
        rc_channel[RC_PITCH] = rc_channel[RC_PITCH] - 90;
        rc_channel[RC_ROLL] = rc_channel[RC_ROLL] - 90;

        /* Kalman filter and PID control */
        pid[ACC_X] = pid_calculate((float) rc_channel[RC_PITCH] * RC_SENSITIVITY, kalman_calculate((float) mpu6050[ACC_X], (float) mpu6050[GYRO_X], looptime, 0), 0);
        pid[ACC_Y] = pid_calculate((float) rc_channel[RC_ROLL] * RC_SENSITIVITY, kalman_calculate((float) mpu6050[ACC_Y], (float) mpu6050[GYRO_Y], looptime, 1), 1);
        pid[ACC_Z] = pid_calculate(0.0, kalman_calculate((float) mpu6050[ACC_Z], (float) mpu6050[GYRO_Z], looptime, 2), 2);

        /* Calculate speeds */
        if (rc_channel[RC_SPEED] > RC_CAP) {
            motor[0] = parseFloat(rc_channel[RC_SPEED] * ((2.0 - pid[ACC_X]) * pid[ACC_Y]));
            motor[1] = parseFloat(rc_channel[RC_SPEED] * (pid[ACC_X] * (2.0 - pid[ACC_Y])));
            motor[2] = parseFloat(rc_channel[RC_SPEED] * (pid[ACC_X] * pid[ACC_Y]));
            motor[3] = parseFloat(rc_channel[RC_SPEED] * (2.0 - (pid[ACC_X] * pid[ACC_Y])));
        } else {
            for (int i = 0; i < 4; ++i) {
                motor[i] = 0;
            }
        }

#endif /* MOTORCONTROL_AVAILABLE */

#ifdef LOG_AVAILABLE
        log_s("M");
        for (int i = 0; i < 7; ++i) {
            log_s(";");
            log_int16_t(mpu6050[i]);
        }
        log_s("\n");

        log_s("P");
        for (int i = 0; i < 4; ++i) {
            log_s(";");
            log_uint16_t((uint16_t)(pid[i]*100));
        }
        log_s("\n");

        log_s("R");
        for (int i = 0; i < 4; ++i) {
            log_s(";");
            log_uint16_t(rc_channel[i]);
        }
        log_s("\n");

        log_s("S");
        for (int i = 0; i < 4; ++i) {
            log_s(";");
            log_uint16_t(motor[i]);
        }
        log_s("\n");
#endif /* LOG_AVAILABLE */

        /* Watchdog reset */
        wdt_reset();
    }

    /* Finally. (Never ever) */
    return 0;
}
