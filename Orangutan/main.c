#include "main.h"

// data

uint8_t schedCount;

struct Data {
  uint8_t ready;
  int countLeft;
  int countRight;
  int motorLeft;
  int motorRight;
  int heading;
  uint8_t distance[SDMIO_NUM_PINS];
} data;

// code

void dataSetup(void) {
  uint8_t i;

  data.ready = 0;
  data.countLeft = 0;
  data.countRight = 0;
  data.motorLeft = 0;
  data.motorRight = 0;
  data.heading = COMPASS_BAD;
  for (i=0;i<SDMIO_NUM_PINS;i++) {
    data.distance[i] = SDMIO_BAD;
  }
}

// indicate data field has been updated
void dataReady(uint8_t ready) {
  data.ready |= ready;
  serialInterrupt();
}

// mark data collected
void dataDone(uint8_t done) {
  data.ready &= ~done;
}

// schedule events
void schedule(void) {
  schedCount++;
/*
  if (((schedCount & 0b00000110) == 0) && (schedCount & 0b1)) {
    // ~32hz
    motorGetCounts();
  } else if ((#schedCount & 0b00011111) == 0)  {
    // ~4hz
    if (schedCount & 0b100000) {
      ping(0);
    } else {
      getCompass();
    }
  }
*/
}

void setup(void) {
  // stop interrupts
  cli();

  // setup for encoder use
  svp_set_mode(SVP_MODE_ENCODERS);

  // setup
  dataSetup();
  lcdSetup();
  serialSetup();
  scheduleSetup(schedule);
  compassSetup();
  sdmioSetup();

  // enable interrupts
  sei();

  // print out start messages
  lcdPrint(LCD_STATUS_ROW, LCD_STATUS_COL, LCD_RUNNING);
  serialPrint(MSG_STARTED);
  serialPrompt(0);
}

void cmdDo(void) {
  char c = cmdBuf[0];
  uint8_t err = FALSE;

  switch (c) {
    // data printout commands
    case 'R':
      sprintf(serialOutBuf, "R %u", data.ready);
      break;
    case 'C':
      sprintf(lcdOutBuf, "C%4d:%4d", data.countLeft, data.countRight);
      lcdPrint(LCD_COUNTS_ROW, LCD_COUNTS_COL, lcdOutBuf);
      sprintf(serialOutBuf, "C %d %d", data.countLeft, data.countRight);
      dataDone(READY_COUNTS);
      break;
    case 'M':
      sprintf(lcdOutBuf, "M%4d:%4d", data.motorLeft, data.motorRight);
      lcdPrint(LCD_MOTOR_ROW, LCD_MOTOR_COL, lcdOutBuf);
      sprintf(serialOutBuf, "M %d %d", data.motorLeft, data.motorRight);
      dataDone(READY_MOTORS);
      break;
    case 'H':
      sprintf(serialOutBuf, "H %d", data.heading);
      dataDone(READY_HEADING);
      break;
    case 'D':
      sprintf(serialOutBuf, "D %u", data.distance[0]);
      dataDone(READY_DISTANCES);
      break;
    // action commands
    case 'm':
      // parse numbers and set motors
      sprintf(serialOutBuf, "Motored");
      break;
    case 'h':
      compassGet();
      data.heading = heading;
      sprintf(lcdOutBuf, "H%3u", data.heading);
      lcdPrint(LCD_HEADING_ROW, LCD_HEADING_COL, lcdOutBuf);
      sprintf(serialOutBuf, "Compassed");
      dataReady(READY_HEADING);
      break;
    case 'd':
      // add getting pin to ping
      if (sdmioPing(0)) {
	sprintf(serialOutBuf, "Pinged");
      } else {
	sprintf(serialOutBuf, "! [%s]", cmdBuf);
	err = TRUE;
      }
      break;
    default:
      sprintf(serialOutBuf, "! [%s]", cmdBuf);
      err = TRUE;
  }
  if (err) {
    lcdPrint(LCD_STATUS_ROW, LCD_STATUS_COL, LCD_ERROR);
  } else {
    lcdPrint(LCD_STATUS_ROW, LCD_STATUS_COL, LCD_RUNNING);
  }
  serialPrint(serialOutBuf);
}

int main(void) {
  uint8_t loops = 0;

  setup();

  while (TRUE) {
    sprintf(lcdOutBuf, "%02X", loops);
    lcdPrint(LCD_COUNT_ROW, LCD_COUNT_COL, lcdOutBuf);

    // check for commands
    if (serialRead()) {
      cmdDo();
      serialPrompt(loops);
    }
    delay_ms(LOOP_DELAY);

    // check for ping results
    if (pinged) {
      if (!pinging) {
	if (id == 0) {
	  sprintf(lcdOutBuf, "D%3u", ticks[id]);
	  lcdPrint(LCD_DISTANCE_0_ROW, LCD_DISTANCE_0_COL, lcdOutBuf);
	}
	data.distance[id] = ticks[id];
	dataReady(READY_DISTANCES);
	pinged = FALSE;
      }
    }

    loops++;
  }
  return FALSE;
}
