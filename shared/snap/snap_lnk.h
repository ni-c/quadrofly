/*
 * Copyright (c) 2003 Jan Kl�tzke
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
 * S.N.A.P link layer
 *
 * The following conditions are assumed about the link-layer:
 *   - calls external function "void snap_lnk_recv(u08 value, u08 err)" when
 *     received a byte
 *   - preamble bytes are managed by the link layer an not passed to network layer
 *   - function "snap_lnk_recv" is called with interrupts disabled
 *   - manages medium access, espacially "snap_lnk_send_start()" may block until
 *     line is free (CSMA)
 *   - while sending "snap_lnk_recv" is nerver called, that is we do not listen
 *     to ourself
 *
 * @file 	snap_def.h
 * @brief 	Definitions of a nearly full implementation of the S.N.A.P protocol from HTH for avr-gcc.
 * @details Library from http://www.avrfreaks.net/index.php?module=Freaks%20Academy&func=viewItem&item_id=156&item_type=project More info about S.N.A.P. can be found on http://www.hth.com/snap/
 * @author  Jan Klötzke (jk177883@inf.tu-dresden.de)
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Mar 11, 2012
 */

#ifndef __SNAP_LNK_H
#define __SNAP_LNK_H

#include <inttypes.h>

#define SNAP_LNK_ERR_FRAMING	1
#define SNAP_LNK_ERR_OVERRUN	2

void snap_lnk_init(void);

void snap_lnk_send_start(void);
void snap_lnk_send(uint8_t value);
void snap_lnk_send_done(void);

#endif
