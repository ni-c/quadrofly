/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	rfm12.c
 * @brief 	Library to communicate with the RFM12 module
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 15, 2012
 */
#include "rfm12.h"
#include "log.h"

#include <avr/io.h>
#include <stdlib.h>

/**
 * Software method to write a 16-bit command over SPI to the RFM12
 *
 * @param cmd The command to write
 * @return The result of the SPI line
 */
unsigned int rfm12_write(unsigned int cmd) {
	unsigned char i;
	unsigned int recv;
	recv = 0;
	PORTSPI &= ~(1 << SCK);
	PORTSPI &= ~(1 << CS);
	for (i = 0; i < 16; i++) {
		if (cmd & 0x8000) {
			PORTSPI |= (1 << SDI);
		} else {
			PORTSPI &= ~(1 << SDI);
		}
		PORTSPI |= (1 << SCK);
		recv <<= 1;
		if (PINSPI & (1 << SDO)) {
			recv |= 0x0001;
		}
		PORTSPI &= ~(1 << SCK);
		cmd <<= 1;
	}
	PORTSPI |= (1 << CS);
	return recv;
}

/**
 * Initializes the RFM12 module
 */
void rfm12_init(void) {
	log_s("rfm12 initialization...");
	DDRSPI &= (1 << SDI) | (1 << SCK) | (1 << CS); // SDI, SCK and CS output
	DDRSPI |= ~(1 << SDO); // SDO  input
	DDRD |= ~(1 << NIRQ); // NIRQ input

	PORTSPI |= (1 << CS); // Pull CS high
	PORTSPI |= (1 << SDI); // Pull SDI high
	PORTSPI &= ~(1 << SCK); // Pull SCK low

	rfm12_write(0x80D7); // Enable FIFO
	rfm12_write(0xC000); // AVR CLK: 10MHz
	rfm12_write(0xC2AB); // Data Filter: internal
	rfm12_write(0xCA81); // Set FIFO mode
	rfm12_write(0xE000); // disable wakeuptimer
	rfm12_write(0xC800); // disable low duty cycle
	rfm12_write(0xC4F7); // AFC settings: autotuning: -10kHz...+7,5kHz
	log_s(" ok\n");
}

/**
 * Set the bandwith, gain and DRSSI threshold of the RFM12
 *
 * Example: rfm12_setbandwidth(4, 1, 4); // 200kHz bandwidth, -6dB gain, DRSSI threshold: -79dBm
 *
 * @param bandwidth The bandwith of the RFM12 module
 * @param gain The gain of the RFM12 module
 * @param drssi The DRSSI threshold of the module
 */
void rfm12_setbandwidth(unsigned char bandwidth, unsigned char gain, unsigned char drssi) {
	rfm12_write(0x9400 | ((bandwidth & 7) << 5) | ((gain & 3) << 3) | (drssi & 7));
}

/**
 * Set the frequency of the RFM12 module
 *
 * Example: rf12_setfreq(RF12FREQ(433.92)); // Set the frequency to 433,92 MHz
 *
 * @param freq The frequency to set
 */
void rfm12_setfreq(unsigned short freq) {
	if (freq < 96) // 430,2400MHz
		freq = 96;
	else if (freq > 3903) // 439,7575MHz
		freq = 3903;
	rfm12_write(0xA000 | freq);
}

/**
 * Set the baudrate of the RFM12 module
 *
 * Example: rfm12_setbaud(19200); // 19200 baud
 *
 * @param baud The baudrate to set
 */
void rfm12_setbaud(unsigned short baud) {
	if (baud < 663)
		return;
	if (baud < 5400) // Baudrate= 344827,58621/(R+1)/(1+CS*7)
		rfm12_write(0xC680 | ((43104 / baud) - 1));
	else
		rfm12_write(0xC600 | ((344828UL / baud) - 1));
}

/**
 * Sets power and frequency shift of the RFM12 module
 *
 * Example: rf12_setpower(0, 6); // 1mW power, 120kHz frequency shift
 *
 * @param power The db value to set
 * @param mod The frequency shift to set
 */
void rfm12_setpower(unsigned char power, unsigned char mod) {
	rfm12_write(0x9800 | (power & 7) | ((mod & 15) << 4));
}

/**
 * Wait until the FIFO register is ready
 */
void rfm12_ready(void) {
	PORTSPI &= ~(1 << CS);
	while (!(PINSPI & (1 << SDO)))
		; // wait until FIFO ready
}

/**
 * Start TX
 */
void rfm12_tx_start(void) {
	log_s("rfm12 TX start...");
	rfm12_write(0x8238); // TX on
	rfm12_ready();
	rfm12_write(0xB8AA);
	rfm12_ready();
	rfm12_write(0xB8AA);
	rfm12_ready();
	rfm12_write(0xB8AA);
	rfm12_ready();
	rfm12_write(0xB82D);
	rfm12_ready();
	rfm12_write(0xB8D4);
	log_s(" ok\n");
}

/**
 * Send data over TX
 *
 * @param value The data to send
 */
void rfm12_tx(uint8_t value) {
	char s[3];
	itoa(value,s,3);
	log_s(s);
	rfm12_ready();
	rfm12_write(0xB800 | (value));
}

/**
 * Finish TX
 */
void rfm12_tx_done(void) {
	log_s("rfm12 TX done...");
	rfm12_ready();
	rfm12_write(0x8208); // TX off
	log_s(" ok\n");
}

/**
 * Start RX
 */
void rfm12_rx_start(void) {
	rfm12_write(0x82C8); // RX on
	rfm12_write(0xCA81); // set FIFO mode
	rfm12_write(0xCA83); // enable FIFO
}

/**
 * Receive RX data
 *
 * @return 2 bytes of data
 */
uint16_t rfm12_rx(void) {
	rfm12_ready();
	return rfm12_write(0xB000);
}

/**
 * Finish RX
 */
void rfm12_rx_done(void) {
	rfm12_write(0x8208); // RX off
}
