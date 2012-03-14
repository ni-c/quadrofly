This archive contains a nearly full implementation of the S.N.A.P protocol from
HTH for avr-gcc. I've used this in my own home automation project but I've
switched to another protocol so this code may no longer be maintained heavily.
I release it to public in the hope that this code is somewhat useful to others.
Look at snap.pdf for a full definition of the protocol.

Architecture:
The code is divided in two parts. The link layer (snap_lnk.c) for all hardware
specific stuff and the network layer (snap.c) which contains the actual snap
protocol. The network layer is written at most device independent and should
easily be ported to other platforms and compilers (IAR?). One drawback of the
current code is that it's heavily cluttered with #ifdefs's. I've done this to
conserve as much ram and flash size as possible but now it's horribly readable.

In my link layer (the one included here) I simply use the internal UART and an
external bus driver for communication. If we want to send a packet the driver
is switched on (PD4). All the other time it is switched off. See the following
schematic for further info. It is somewhat simplified but should explain the
function...

        |-------------|
        |  at90s8515  |
        |             |
        |         PD4 +-------------|
        |         PD3 +-          |\o
        |         PD2 +-          | \
        |    (TXD)PD1 +-----------|  >------+-----> bus
        |    (RXD)PD0 +---|       | /       |
        |             |   |       |/        |
        |-------------|   |                 |
                          |-----------------|


IMPORTANT:
To compile these files for your MCU have a look at snap_def.h. This file
contains many definitions which affect the code size and speed. I've written
many comments so the switches should be fairly understandable.

For bug reports, comments and donations write at:
Jan Klötzke <jk177883@inf.tu-dresden.de>


Have fun...
