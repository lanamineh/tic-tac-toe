/**
 * This file has defines for the commands in Chapter 10 of the manual.
 * I've only included commands that the board is capable of doing,
 * there are no read commands because reading from the display is not supported.
 */

#ifndef COMMANDS_H
#define COMMANDS_H

// Table 10.1.1
#define NOP 0x00
#define SWRESET 0x01

// Table 10.1.2
#define SLPIN 0x10
#define SLPOUT 0x11
#define PTLON 0x12
#define NORON 0x13
#define INVOFF 0x20
#define INVON 0x21
#define GAMSET 0x26
#define DISPOFF 0x28
#define DISPON 0x29
#define CASET 0x2a
#define RASET 0x2b
#define RAMWR 0x2c

// Table 10.1.3
#define PTLAR 0x30
#define TEOFF 0x34
#define TEON 0x35
#define MADCTL 0x36
#define IDMOFF 0x38
#define IDMON 0x39
#define COLMOD 0x3a

#endif
