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
#include "motor.h"

// time to delay between loops
#define LOOP_DELAY 50
#define SUB_LOOP_MAX 40

// ready flags
#define READY_INIT 0
#define READY_COUNTS 0b00000001
#define READY_MOTORS 0b00000010
#define READY_HEADING 0b00000100
#define READY_DISTANCES 0b00001000

// messages
#define MSG_STARTED "Started"
#define MSG_MOTORED "Motored"
#define MSG_COMPASSED "Compassed"
#define MSG_PINGED "Pinged"

// function characters
#define LCD_RUNNING "$"
#define LCD_ERROR "!"

// cmd constants
#define FUNCTION_POS 0
#define ARGS_POS 1
#define CMD_m_ARGS 2
#define CMD_d_ARGS 1

// lcd sprintf
#define LCD_COUNT_SPR "C%4d:%4d"
#define LCD_MOTOR_SPR "M%4d:%4d"
#define LCD_DISTANCE_SPR "D%3u"
#define LCD_HEADING_SPR "H%3u"
#define LCD_LOOP_SPR "%02X"

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

// functions
void dataSetup(void);
void dataReady(uint8_t ready);
void dataDone(uint8_t done);
void setup(void);
void cmdDo(void);
int main(void);

#endif // MAIN_H
