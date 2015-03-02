#include "compass.h"

int heading;

void compassSetup(void) {
  i2c_init();
  heading = COMPASS_BAD;
}

void compassGet(void) {
  uint8_t low, high;
  heading = COMPASS_BAD;

  // tell comppas to measure
  i2c_start_wait(COMPASS_ADDR + I2C_WRITE);
  i2c_write(COMPASS_CMD);
  // wait and read response
  delay_ms(COMPASS_DELAY);
  i2c_start_wait(COMPASS_ADDR + I2C_READ);
  high = i2c_readAck();
  low = i2c_readAck();
  // round to the nearest degree
  heading = ((high << BYTE_LENGTH) + low + ROUND_SUM) / ROUND_DIVIDE;
}
