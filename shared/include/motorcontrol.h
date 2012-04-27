/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file 	motorcontrol.h
 * @brief 	Motor control definitions
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 27, 2012
 */

#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_


#define MC_ENABLE           0x00 /*!< I2C address enable/disable */
#define MC_MOTOR_1_SPEED    0x01 /*!< I2C address for the speed of motor 1 */
#define MC_MOTOR_2_SPEED    0x02 /*!< I2C address for the speed of motor 2 */
#define MC_MOTOR_3_SPEED    0x03 /*!< I2C address for the speed of motor 3 */
#define MC_MOTOR_4_SPEED    0x04 /*!< I2C address for the speed of motor 4 */
#define MC_RC_CHANNEL_1     0x05 /*!< I2C address for the value of channel 1 of the RC RX */
#define MC_RC_CHANNEL_2     0x06 /*!< I2C address for the value of channel 2 of the RC RX */
#define MC_RC_CHANNEL_3     0x07 /*!< I2C address for the value of channel 3 of the RC RX */
#define MC_RC_CHANNEL_4     0x08 /*!< I2C address for the value of channel 4 of the RC RX */
#define MC_RC_CHANNEL_5     0x09 /*!< I2C address for the value of channel 5 of the RC RX */
#define MC_RC_CHANNEL_6     0x10 /*!< I2C address for the value of channel 6 of the RC RX */
#define MC_RC_CHANNEL_7     0x11 /*!< I2C address for the value of channel 7 of the RC RX */
#define MC_RC_CHANNEL_8     0x12 /*!< I2C address for the value of channel 8 of the RC RX */


#endif /* MOTORCONTROL_H_ */
