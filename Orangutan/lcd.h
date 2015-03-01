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

// variables
extern char lcdOutBuf[LCD_LINE_SIZE];

// functions
void lcdSetup(void);
void lcdPrint(uint8_t row, uint8_t col, char *str);

#endif // LCD_H
