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

#ifdef RFM12B_AVAILABLE
/**
 * Received a rf12b packet
 *
 * @param value The packet data
 */
void rfm12_receive(uint8_t value) {

}
#endif /* RFM12B_AVAILABLE */

uint8_t speed = 0x00;

#ifdef MPU6050_AVAILABLE
int16_t mpu6050[7];
#endif /* MPU6050_AVAILABLE */

/**
 * The main function.
 */
int main(void) {

    /* Initialization */
    init_qfly();
    log_s("up and running\n");
    motor_set(0x01, 0x01, 0x01, 0x01);

    /* Our loop */
    while (1) {

#ifdef MPU6050_AVAILABLE
        mpu6050_getall(&mpu6050[ACC_X], &mpu6050[ACC_Y], &mpu6050[ACC_Z], &mpu6050[GYRO_X], &mpu6050[GYRO_Y], &mpu6050[GYRO_Z], &mpu6050[TEMP]);
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
            uart_tx_int16_t(millis());
            uart_tx("\n");
        }
#endif /* UART_AVAILABLE */
    }

    /* Finally. (Never ever) */
    return 0;
}



/* TODO Kalman filter */

float Q_angle = 0.001;  //0.001
float Q_gyro = 0.003;  //0.003
float R_angle = 0.03;  //0.03

float x_angle = 0;
float x_bias = 0;
float P_00 = 0, P_01 = 0, P_10 = 0, P_11 = 0;
float dt, y, S;
float K_0, K_1;

float kalmanCalculate(float newAngle, float newRate, int looptime) {
    dt = (float)looptime/1000;
    x_angle += dt * (newRate - x_bias);
    P_00 += -dt * (P_10 + P_01) + Q_angle * dt;
    P_01 += -dt * P_11;
    P_10 += -dt * P_11;
    P_11 += +Q_gyro * dt;

    y = newAngle - x_angle;
    S = P_00 + R_angle;
    K_0 = P_00 / S;
    K_1 = P_10 / S;

    x_angle += K_0 * y;
    x_bias += K_1 * y;
    P_00 -= K_0 * P_00;
    P_01 -= K_0 * P_01;
    P_10 -= K_1 * P_00;
    P_11 -= K_1 * P_01;

    return x_angle;
}

