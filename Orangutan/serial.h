#ifndef SERIAL_H
#define SERIAL_H

// main includes
#include "base.h"

// std inlcudes
#include <string.h>

// serial constants
#define SERIAL_BUFFER_SIZE 32
#define BAUD 57600
#define LF "\n"

//variables
extern char serialOutBuf[SERIAL_BUFFER_SIZE];
extern char cmdBuf[SERIAL_BUFFER_SIZE];

//functions
void serialSetup(void);
void serialPrint(char *str);
uint8_t serialRead(void);
void serialInterrupt(void);

#endif // SERIAL_H

