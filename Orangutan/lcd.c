#include "lcd.h"

char lcdOutBuf[LCD_LINE_SIZE];

void lcdSetup(void) {
  clear();
}

void lcdPrint(uint8_t row, uint8_t col, char *str) {

  lcd_goto_xy(col, row);
  print(str);
}
