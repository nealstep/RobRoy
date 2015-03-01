#ifndef MAIN_H
#define MAIN_H

// main includes
#include "base.h"

// std includes
#include <stdio.h>
#include <util/atomic.h>

// local includes
#include "lcd.h"
#include "serial.h"
#include "schedule.h"
#include "compass.h"
#include "sdmio.h"

// time to delay between loops
#define LOOP_DELAY 2000

// ready flags
#define READY_COUNTS 0b00000001
#define READY_MOTORS 0b00000010
#define READY_HEADING 0b00000100
#define READY_DISTANCES 0b00001000

// messages
#define MSG_STARTED "Started"

// function characters
#define LCD_RUNNING "$"
#define LCD_ERROR "!"

// functions
void dataSetup(void);
void dataReady(uint8_t ready);
void dataDone(uint8_t done);
void setup(void);
void cmdDo(void);
int main(void);

#endif // MAIN_H
