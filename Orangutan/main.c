#include "main.h"

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
  data.ready -= done;
}

// schedule events
void schedule(void) {
  schedCount++;
/*
  if (((schedCount & 0b110) == 0) && (schedCount & 0b1)) {
    // ~32hz
    checkSync();
    motorGetCounts();
  } else if ((#schedCount & 0b11111) == 0)  {
    // ~4hz
    if (schedCount & 0b100000) {
      ping(0);
    } else {
      getCompass();
      checkCourse();
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

  // print out started
  lcdPrint(0, 0, "Started");
  lcdPrint(1, 0, " ");
  serialPrint("Started");
  sprintf("%3u > ", serialOutBuf, 0);
  serialPrint(serialOutBuf);
}

void cmdDo(void) {
  uint8_t i;
  char c = cmdBuf[0];
  uint8_t err = FALSE;

  switch (c) {
    // data printout commands
    case 'R':
      sprintf(serialOutBuf, "R %u", data.ready);
      break;
    case 'C':
      sprintf(serialOutBuf, "C %d %d", data.countLeft, data.countRight);
      dataDone(COUNTS);
      break;
    case 'M':
      sprintf(serialOutBuf, "M %d %d", data.motorLeft, data.motorRight);
      dataDone(MOTORS);
      break;
    case 'H':
      sprintf(serialOutBuf, "H %d", data.heading);
      dataDone(HEADING);
      break;
    case 'D':
      for (i=0;i<SDMIO_NUM_PINS;i++) {
	sprintf(serialOutBuf, "D %u", data.distance[i]);
      }
      dataDone(DISTANCES);
      break;
    // action commands
    case 'm':
      // parse numbers and set motors
      break;
    case 'h':
      compassGet();
      data.heading = heading;
      dataReady(HEADING);
      break;
    case 'd':
      // add getting pin to ping
      if (sdmioPing(0)) {
	sprintf(serialOutBuf, "! [%s]", cmdBuf);
	err = TRUE;
      }
      break;
    default:
      sprintf(serialOutBuf, "! [%s]", cmdBuf);
      err = TRUE;
  }
  if (err) {
    lcdPrint(1, 0, "!");
  }
  serialPrint(serialOutBuf);
}

int main(void) {
  uint8_t loops = 0;

  setup();

  while (TRUE) {
    sprintf("%3u", lcdOutBuf, loops);
    lcdPrint(1, 1, lcdOutBuf);
    // check for commands
    if (serialRead()) {
      cmdDo();
      sprintf("%3u > ", serialOutBuf, loops);
      serialPrint(serialOutBuf);
    }
    delay_ms(LOOP_DELAY);
    // check for ping results
    if (pinged) {
      if (!pinging) {
	sprintf(lcdOutBuf, "D %3u", ticks[id]);
	lcdPrint(1, 11, lcdOutBuf);
	data.distance[id] = ticks[id];
	dataReady(DISTANCES);
      }
    }
    loops++;
  }
  return FALSE;
}
