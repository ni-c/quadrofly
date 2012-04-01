/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	rfm12.h
 * @brief 	Header file for the RFM12 module
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 15, 2012
 */

#ifndef RFM12_H_
#define RFM12_H_

#include <inttypes.h>

#define RFM12_BUFFER_SIZE 8 /*!< Size of the RFM12 buffer 2..254 */

#define RF12FREQ(freq)	((freq-430.0)/0.0025) /*!< macro for calculating frequency value out of frequency in MHz */

/* RFM12 Interface */
#define DDRSPI	DDRB	/*!< SPI Data Direction Register */
#define PORTSPI PORTB   /*!< SPI Port */
#define PINSPI	PINB	/*!< SPI Pin */

#define NIRQ	PD2		/*!< Interrupts request output(active low) */

#if defined __AVR_ATmega1284P__ || __AVR_ATmega644P__
#define CS		PB4		/*!< SPI SS (Chip select (active low) */
#define SDI		PB5		/*!< SPI Data input (RFM12 side) */
#define SDO		PB6		/*!< SPI Data output (RFM12 side) */
#define SCK		PB7		/*!< SPI clock output */
#else // if defined __AVR_ATmega1284P__ || __AVR_ATMEGA644P__
#define CS		PB2		/*!< SPI SS (Chip select (active low) */
#define SDI		PB3		/*!< SPI Data input (RFM12 side) */
#define SDO		PB4		/*!< SPI Data output (RFM12 side) */
#define SCK		PB5		/*!< SPI clock output */
#endif // if defined __AVR_ATmega1284P__ || __AVR_ATMEGA644P__

/**
 * Initializes the RFM12 module
 */
void rfm12_init(void);

/**
 * Send the value over the RFM12B module
 *
 * @param data The array of char to send
 */
void rfm12_send(uint8_t *data);

/**
 * Set the bandwith, gain and DRSSI threshold of the RFM12
 *
 * Example: rfm12_setbandwidth(4, 1, 4); // 200kHz bandwidth, -6dB gain, DRSSI threshold: -79dBm
 *
 * @param bandwidth The bandwith of the RFM12 module
 * @param gain The gain of the RFM12 module
 * @param drssi The DRSSI threshold of the module
 */
void rfm12_setbandwidth(uint8_t bandwidth, uint8_t gain, uint8_t drssi);

/**
 * Set the frequency of the RFM12 module
 *
 * Example: rf12_setfreq(RF12FREQ(433.92)); // Set the frequency to 433,92 MHz
 *
 * @param freq The frequency to set
 */
void rfm12_setfreq(unsigned short freq);

/**
 * Set the baudrate of the RFM12 module
 *
 * Example: rfm12_setbaud(19200); // 19200 baud
 *
 * @param baud The baudrate to set
 */
void rfm12_setbaud(unsigned short baud);

/**
 * Sets power and frequency shift of the RFM12 module
 *
 * Example: rf12_setpower(0, 6); // 1mW power, 120kHz frequency shift
 *
 * @param power The db value to set
 * @param mod The frequency shift to set
 */
void rfm12_setpower(uint8_t power, uint8_t mod);

/**
 * Enable RX
 */
void rfm12_rx_on(void);

/**
 * Disable RX
 */
void rfm12_rx_off(void);

/**
 * Receive RX data
 *
 * @param data Pointer to the data char array
 * @return The received data
 */
uint8_t *rfm12_rx(uint8_t *data);

/**
 * Software method to write a 16-bit command over SPI to the RFM12
 *
 * @param cmd The command to write
 * @return The result of the SPI line
 */
unsigned int rfm12_write(unsigned int cmd);

/**
 * Send data over TX
 *
 * @param data The array of char to send
 */
void rfm12_tx(uint8_t *data);

/**
 * Reset the FIFO
 */
void fifo_reset(void);

#endif /* RFM12_H_ */
