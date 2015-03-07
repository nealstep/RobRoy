#ifndef SERIAL_H
#define SERIAL_H

// main includes
#include "base.h"

// std inlcudes
#include <stdio.h>
#include <string.h>

// serial constants
#define SERIAL_BUFFER_SIZE 32
#define SERIAL_BAUD 57600
#define SERIAL_INTERRUPT _BV(PB3)
#define SERIAL_PROMPT_SPR "%02X> "

// serial sprintf
#define SERIAL_R_SPR "R %u"
#define SERIAL_COUNT_SPR "C %d %d"
#define SERIAL_MOTOR_SPR "M %d %d"
#define SERIAL_HEADING_SPR "H %d"
#define SERIAL_DISTANCE_SPR "D%1u %u"
#define SERIAL_ERROR_SPR "! [%s]"

// serial input scanf
#define SCANF_MOTOR "%d %d"
#define SCANF_DISTANCE "%hhu"

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

