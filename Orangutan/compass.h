#ifndef COMPASS_H
#define COMPASS_H

// main includes
#include "base.h"

// std includes
#include <util/twi.h>

// local includes
#include "twimaster/i2cmaster.h"

// constants
#define COMPASS_ADDR 0x42
#define COMPASS_CMD 'A'
#define COMPASS_DELAY 6 // process time in ms
#define COMPASS_BAD -1
#define BYTE_LENGTH 8
#define ROUND_SUM 5
#define ROUND_DIVIDE 10

// variables
extern int heading;

// functions
void compassSetup(void);
void compassGet(void);

#endif // COMPASS_H
