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

#include <avr/io.h>

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
uint16_t mpu6050[7];
#endif /* MPU6050_AVAILABLE */

/**
 * The main function.
 */
int main(void) {

    /* Initialization */
    init_qfly();
    motor_set(0x01, 0x01, 0x01, 0x01);
    log_s("up and running\n");
    motor_set(0x00, 0x00, 0x00, 0x00);

    /* Our loop */
    while (1) {

#ifdef MPU6050_AVAILABLE
        /* MPU-6050 update */
        if (mpu6050_data_ready()) {
            mpu6050_getall(mpu6050[ACC_X], mpu6050[ACC_Y], mpu6050[ACC_Z], mpu6050[GYRO_X], mpu6050[GYRO_Y], mpu6050[GYRO_Z], mpu6050[TEMP]);
        }
#endif /* MPU6050_AVAILABLE */

#ifdef UART_AVAILABLE
        if (uart_tx_ready()) {
            uart_tx(mpu6050[ACC_X]);
            uart_tx(mpu6050[ACC_X] >> 8);
            uart_tx(mpu6050[ACC_Y]);
            uart_tx(mpu6050[ACC_Y] >> 8);
            uart_tx(mpu6050[ACC_Z]);
            uart_tx(mpu6050[ACC_Z] >> 8);
            uart_tx(mpu6050[GYRO_X]);
            uart_tx(mpu6050[GYRO_X] >> 8);
            uart_tx(mpu6050[GYRO_Y]);
            uart_tx(mpu6050[GYRO_Y] >> 8);
            uart_tx(mpu6050[GYRO_Z]);
            uart_tx(mpu6050[GYRO_Z] >> 8);
        }
#endif /* UART_AVAILABLE */
    }

    /* Finally. (Never ever) */
    return 0;
}
