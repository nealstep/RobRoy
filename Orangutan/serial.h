#ifndef SERIAL_H
#define SERIAL_H

// main includes
#include "base.h"

// std inlcudes
#include <stdio.h>
#include <string.h>

// serial constants
#define SERIAL_BUFFER_SIZE 32
#define BAUD 57600
#define LF "\n"
#define SERIAL_INTERRUPT _BV(PB3)

//variables
extern char serialOutBuf[SERIAL_BUFFER_SIZE];
extern char cmdBuf[SERIAL_BUFFER_SIZE];

//functions
void serialSetup(void);
void serialPrint(char *message);
void serialPrompt(uint8_t loops);
uint8_t serialRead(void);
void serialInterrupt(void);

#endif // SERIAL_H

