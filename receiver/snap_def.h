/*
 * Copyright (c) 2003 Jan Klötzke
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *
 * @file 	snap_def.h
 * @brief 	Definitions of a nearly full implementation of the S.N.A.P protocol from HTH for avr-gcc.
 * @details Library from http://www.avrfreaks.net/index.php?module=Freaks%20Academy&func=viewItem&item_id=156&item_type=project More info about S.N.A.P. can be found on http://www.hth.com/snap/
 * @author  Jan Klötzke (jk177883@inf.tu-dresden.de)
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 11, 2012
 */
#ifndef SNAP_DEF_H_
#define SNAP_DEF_H_

/*
 * Network layer definitions.
 */

/**
 * Define the following symbol to make the network layer do extra checks
 * on packet integrity. This affects code size and speed but ensures that packets
 * which are not supported (features, size, ...) do nothing harmfull. Not defining
 * this constant makes the protocol stack vulnerable to buffer overruns!!!
 * Nevertheless the code could contain further security flaws...
 */
#define SNAP_SECURE 

/**
 * The maximum packet length only affects the maximum received packet
 * size, that is the receive buffer size. You can still send packets larger
 * than SNAP_MAX_PACKET_LENGTH but the internal length counter is only as
 * wide as SNAP_MAX_PACKET_LENGTH is.
 */
#define SNAP_MAX_PACKET_LENGTH	32	/*!< Value between 0..512 */
#define SNAP_MAX_ADDRESS_BYTES	1	/*!< Value between 0..3   */
#define SNAP_MAX_PROTOCOL_BYTES	0	/*!< Value between 0..3   */
 
#define SNAP_STATIC_ADDR	0x12

/**
 * Pay attantion that espacially the good EDM routines need more processing
 * time. Since the CRC's are calculated on the fly this may lead to overrun
 * errors if the baud rate is too high! 
 */
//#define SNAP_EDM_CHECKSUM_8
#define SNAP_EDM_CRC_8
//#define SNAP_EDM_CRC_16
//#define SNAP_EDM_CRC_32
//#define SNAP_EDM_FEC			/* not supported */

/**
 * Specify the byte receive timeout in timer ticks (snap_check_timeout).
 * Adjust according to your timer interval and baud rate. A good choice
 * should be 3*(time between bytes).
 * Range: 0..255
 */
#define SNAP_NET_TIMEOUT	64

/*
 * Link layer definitions.
 */

/**
 * Specify your desired baud rate and current master clock frequency.
 * Note that the actual baud rate may differ. (see manual)
 * Also keep in mind that high rates may not be feasible due to edm and
 * protocol processing time and other interrupts.
 */
#define SNAP_BAUD_RATE		9600		/*!< desired baud rate    */
#define SNAP_F_CLK			F_CPU       /*!< your clock frequency */

#endif
