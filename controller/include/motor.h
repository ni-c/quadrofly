/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file 	/quadrofly/controller/include/motor.h/motor.h
 * @brief 	
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 19, 2012
 */

#ifndef MOTOR_H_
#define MOTOR_H_

/**
 * Set the speed of the motors
 *
 * @param a Speed of motor A
 * @param b Speed of motor B
 * @param c Speed of motor C
 * @param d Speed of motor D
 * @return If the setting was succesful
 */
uint8_t motor_set(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

#endif /* MOTOR_H_ */
