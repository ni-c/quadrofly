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

#include <avr/io.h>
#include <util/delay.h>

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
    log_s("initialization finished\n");

    /* Our loop */
    while (1) {

#ifdef UART_AVAILABLE
        if (uart_rx_ready()) {
            char *c = uart_rx();
            if (c[0]=='0') {
                if (i2c_start(I2C_ADDR_MOTORCONTROL - 1 + I2C_WRITE)) {
                    i2c_write(0x00);
                    i2c_write(0x00);
                    i2c_write(0x00);
                    i2c_write(0x00);
                    i2c_write(0x00);
                    i2c_stop();
                    uart_tx("\nSetting motor speed to 0\n");
                } else {
                    uart_tx("I2C Error\n");
                }
            }
            if (c[0]=='1') {
                if (i2c_start(I2C_ADDR_MOTORCONTROL - 1 + I2C_WRITE)) {
                    i2c_write(0x00);
                    i2c_write(0x20);
                    i2c_write(0x20);
                    i2c_write(0x20);
                    i2c_write(0x20);
                    i2c_stop();
                    uart_tx("\nSetting motor speed to 32\n");
                } else {
                    uart_tx("I2C Error\n");
                }
            }
            if (c[0]=='2') {
                if (i2c_start(I2C_ADDR_MOTORCONTROL - 1 + I2C_WRITE)) {
                    i2c_write(0x00);
                    i2c_write(0x40);
                    i2c_write(0x40);
                    i2c_write(0x40);
                    i2c_write(0x40);
                    i2c_stop();
                    uart_tx("\nSetting motor speed to 64\n");
                } else {
                    uart_tx("I2C Error\n");
                }
            }
            if (c[0]=='3') {
                if (i2c_start(I2C_ADDR_MOTORCONTROL - 1 + I2C_WRITE)) {
                    i2c_write(0x00);
                    i2c_write(0x80);
                    i2c_write(0x80);
                    i2c_write(0x80);
                    i2c_write(0x80);
                    i2c_stop();
                    uart_tx("\nSetting motor speed to 128\n");
                } else {
                    uart_tx("I2C Error\n");
                }
            }
            if (c[0]=='4') {
                if (i2c_start(I2C_ADDR_MOTORCONTROL - 1 + I2C_WRITE)) {
                    i2c_write(0x00);
                    i2c_write(0xFE);
                    i2c_write(0xFE);
                    i2c_write(0xFE);
                    i2c_write(0xFE);
                    i2c_stop();
                    uart_tx("\nSetting motor speed to 254\n");
                } else {
                    uart_tx("I2C Error\n");
                }
            }
        }
#endif /* UART_AVAILABLE */
    }

    /* Finally. (Never ever) */
    return 0;
}
