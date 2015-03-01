#ifndef LCD_H
#define LCD_H

// main includes
#include "base.h"

// lcd constants
#define LCD_LINE_SIZE 16

// variables
extern char lcdOutBuf[LCD_LINE_SIZE];

// functions
void lcdSetup(void);
void lcdPrint(uint8_t row, uint8_t col, char *str);

#endif // LCD_H
