#ifndef SDMIO_H
#define SDMIO_H

// base include
#include "base.h"

// constants
#define SDMIO_WARM_TIME 25
#define SDMIO_SIGNAL_TIME 12
#define SDMIO_NUM_PINS 1
#define SDMIO_BAD 250
#define SDMIO_MAIN 0
#define SDMIO_TIMER_CONTROL_INIT 0

// local variables
extern volatile uint8_t ticks[SDMIO_NUM_PINS];
extern volatile uint8_t pinging;
extern volatile uint8_t pinged;
extern volatile uint8_t id;

// local functions
void sdmioSetup(void);
uint8_t sdmioPing(uint8_t pin);

#endif //SDMIO_H
