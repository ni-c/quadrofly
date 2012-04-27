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
#include "init.h"
#include "uart.h"
#include "log.h"
#include "rfm12.h"
#include "i2cmaster.h"
#include "mpu6050.h"
#include "motor.h"
#include "millis.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#ifdef MPU6050_AVAILABLE
int16_t mpu6050[7]; /*!< MPU-6050 measurements */
float kalman[3]; /*!< Kalman Filter results */
#endif /* MPU6050_AVAILABLE */

uint32_t lastlooptime = 0; /*!< Timestamp of the last loop */
uint8_t looptime = 0; /*!< Loop time in ms */


/*
 * ==========================================================================================================
 * TODO Kalman filter
 * ==========================================================================================================
 */

#define Q_ANGLE 0.001
#define Q_GYRO  0.003
#define R_ANGLE 0.03

float kalman_angle[3] = {0, 0, 0};
float kalman_bias[3] = {0, 0, 0};
float kalman_P_00[3] = {0, 0, 0};
float kalman_P_01[3] = {0, 0, 0};
float kalman_P_10[3] = {0, 0, 0};
float kalman_P_11[3] = {0, 0, 0};
float kalman_y[3] = {0, 0, 0};
float kalman_S[3] = {0, 0, 0};
float kalman_K_0[3] = {0, 0, 0};
float kalman_K_1[3] = {0, 0, 0};

float kalmanCalculate(float acc, float gyro, uint8_t looptime, uint8_t key) {

    float kalman_dt = (float)looptime/1000;

    kalman_angle[key] += kalman_dt * (gyro - kalman_bias[key]);
    kalman_P_00[key] += -kalman_dt * (kalman_P_10[key] + kalman_P_01[key]) + Q_ANGLE * kalman_dt;
    kalman_P_01[key] += -kalman_dt * kalman_P_11[key];
    kalman_P_10[key] += -kalman_dt * kalman_P_11[key];
    kalman_P_11[key] += +Q_GYRO * kalman_dt;

    kalman_y[key] = acc - kalman_angle[key];
    kalman_S[key] = kalman_P_00[key] + R_ANGLE;
    kalman_K_0[key] = kalman_P_00[key] / kalman_S[key];
    kalman_K_1[key] = kalman_P_10[key] / kalman_S[key];

    kalman_angle[key] += kalman_K_0[key] * kalman_y[key];
    kalman_bias[key] += kalman_K_1[key] * kalman_y[key];
    kalman_P_00[key] -= kalman_K_0[key] * kalman_P_00[key];
    kalman_P_01[key] -= kalman_K_0[key] * kalman_P_01[key];
    kalman_P_10[key] -= kalman_K_1[key] * kalman_P_00[key];
    kalman_P_11[key] -= kalman_K_1[key] * kalman_P_01[key];

    return kalman_angle[key];
}

/*
 * ==========================================================================================================
 */

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
 * The main function.
 */
int main(void) {

    /* Initialization */
    init_qfly();
    log_s("up and running\n");

    /* Our loop */
    while (1) {

        looptime = millis() - lastlooptime;
        lastlooptime = millis();

#ifdef MPU6050_AVAILABLE

        /* Read MPU-6050 measurements */
        mpu6050_getall(&mpu6050[ACC_X], &mpu6050[ACC_Y], &mpu6050[ACC_Z], &mpu6050[GYRO_X], &mpu6050[GYRO_Y], &mpu6050[GYRO_Z], &mpu6050[TEMP]);

        /* Kalman filter the MPU-6050 measurements */
        for (int i = 0; i < 3; i++) {
            kalman[i] = kalmanCalculate((float)mpu6050[i], (float)mpu6050[3+i], looptime, i);
        }

#endif /* MPU6050_AVAILABLE */


#ifdef UART_AVAILABLE
        if (uart_tx_ready()) {
            uart_tx_int16_t(mpu6050[ACC_X]);
            uart_tx(";");
            uart_tx_int16_t(mpu6050[ACC_Y]);
            uart_tx(";");
            uart_tx_int16_t(mpu6050[ACC_Z]);
            uart_tx(";");
            uart_tx_int16_t(mpu6050[GYRO_X]);
            uart_tx(";");
            uart_tx_int16_t(mpu6050[GYRO_Y]);
            uart_tx(";");
            uart_tx_int16_t(mpu6050[GYRO_Z]);
            uart_tx(";");
            uart_tx_int16_t((int16_t)kalman[0]);
            uart_tx(";");
            uart_tx_int16_t((int16_t)kalman[1]);
            uart_tx(";");
            uart_tx_int16_t((int16_t)kalman[2]);
            uart_tx(";");
            uart_tx("\n");
        }
#endif /* UART_AVAILABLE */
    }

    /* Finally. (Never ever) */
    return 0;
}
