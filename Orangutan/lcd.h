#ifndef LCD_H
#define LCD_H

// main includes
#include "base.h"

// lcd constants
#define LCD_LINE_SIZE 16
#define LCD_STATUS_ROW 0
#define LCD_STATUS_COL 0
#define LCD_MOTOR_ROW 0
#define LCD_MOTOR_COL 2
#define LCD_HEADING_ROW 0
#define LCD_HEADING_COL 12
#define LCD_COUNT_ROW 1
#define LCD_COUNT_COL 0
#define LCD_COUNTS_ROW 1
#define LCD_COUNTS_COL 2
#define LCD_DISTANCE_0_ROW 1
#define LCD_DISTANCE_0_COL 12

// function characters
#define LCD_RUNNING "$"
#define LCD_ERROR "!"

// lcd sprintf
#define LCD_COUNT_SPR "C%4d:%4d"
#define LCD_MOTOR_SPR "M%4d:%4d"
#define LCD_DISTANCE_SPR "D%3u"
#define LCD_HEADING_SPR "H%3u"
#define LCD_LOOP_SPR "%02X"

// variables
extern char lcdOutBuf[LCD_LINE_SIZE];

// functions
void lcdSetup(void);
void lcdPrint(uint8_t row, uint8_t col, char *str);

#endif // LCD_H
