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

int speed[MOTOR_COUNT];
uint8_t sensor;

// code

void dataSetup(void) {
  uint8_t i;

  data.ready = READY_INIT;
  data.countLeft = MOTOR_COUNT_INIT;
  data.countRight = MOTOR_COUNT_INIT;
  data.motorLeft = MOTOR_INIT;
  data.motorRight = MOTOR_INIT;
  data.heading = COMPASS_BAD;
  for (i=ZERO;i<SDMIO_NUM_PINS;i++) {
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
/* need to put binary values into constants
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
  motorSetup();

  // enable interrupts
  sei();

  // print out start messages
  lcdPrint(LCD_STATUS_ROW, LCD_STATUS_COL, LCD_RUNNING);
  serialPrint(MSG_STARTED);
}

void getCountF(void) {
  motorGetCounts();
  data.countLeft = countLeft;
  data.countRight = countRight;
  dataReady(READY_COUNTS);
  sprintf(lcdOutBuf, LCD_COUNT_SPR, data.countLeft, data.countRight);
  lcdPrint(LCD_COUNTS_ROW, LCD_COUNTS_COL, lcdOutBuf);
}

void getCompassF(void) {
  compassGet();
  data.heading = heading;
  dataReady(READY_HEADING);
  sprintf(lcdOutBuf, LCD_HEADING_SPR, data.heading);
  lcdPrint(LCD_HEADING_ROW, LCD_HEADING_COL, lcdOutBuf);
}

void cmdDo(void) {
  uint8_t v, i;
  char c = cmdBuf[FUNCTION_POS];
  uint8_t err = FALSE;

  if (!c) {
    return;
  }

  switch (c) {
    // data printout commands
    case 'R':
      sprintf(serialOutBuf, SERIAL_R_SPR, data.ready);
      serialPrint(serialOutBuf);
      break;
    case 'C':
      sprintf(serialOutBuf, SERIAL_COUNT_SPR, data.countLeft, data.countRight);
      serialPrint(serialOutBuf);
      dataDone(READY_COUNTS);
      break;
    case 'M':
      sprintf(serialOutBuf, SERIAL_MOTOR_SPR, data.motorLeft, data.motorRight);
      serialPrint(serialOutBuf);
      dataDone(READY_MOTORS);
      break;
    case 'H':
      sprintf(serialOutBuf, SERIAL_HEADING_SPR, data.heading);
      serialPrint(serialOutBuf);
      dataDone(READY_HEADING);
      break;
    case 'D':
      for (i=ZERO;i<SDMIO_NUM_PINS;i++) {
	sprintf(serialOutBuf, SERIAL_DISTANCE_SPR, i, data.distance[i]);
	serialPrint(serialOutBuf);
      }
      dataDone(READY_DISTANCES);
      break;
    // action commands
    case 'c':
      getCountF();
      break;
    case 'm':
      v = sscanf(&cmdBuf[ARGS_POS], SCANF_MOTOR, 
		 &speed[MOTOR_LEFT], &speed[MOTOR_RIGHT]);
      if (v == CMD_m_ARGS) {
	for (i=0;i<MOTOR_COUNT;i++) {
	  if (speed[i] < -UINT8_T_MAX) {
	    speed[i] = -UINT8_T_MAX;
	  } else if (speed[i] > UINT8_T_MAX) {
	    speed[i] = UINT8_T_MAX;
	  }
	}
	motorSetSpeeds(speed[MOTOR_LEFT], speed[MOTOR_RIGHT]);
	data.motorLeft = motorLeft;
	data.motorRight = motorRight;
	dataReady(READY_MOTORS);
	sprintf(lcdOutBuf, LCD_MOTOR_SPR, data.motorLeft, data.motorRight);
	lcdPrint(LCD_MOTOR_ROW, LCD_MOTOR_COL, lcdOutBuf);
      } else {
	err = TRUE;
      }
      break;
    case 'h':
      getCompassF();
      break;
    case 'd':
      v = sscanf(&cmdBuf[1], SCANF_DISTANCE, &sensor);
      if (v == CMD_d_ARGS) {
	if (sensor < SDMIO_NUM_PINS) {
	  if (sdmioPing(sensor)) {
	    sprintf(serialOutBuf, MSG_PINGED);
	    serialPrint(serialOutBuf);
	    break;
	  }
	}
      }
    default:
      err = TRUE;
  }
  if (err) {
    sprintf(serialOutBuf, SERIAL_ERROR_SPR, cmdBuf);
    lcdPrint(LCD_STATUS_ROW, LCD_STATUS_COL, LCD_ERROR);
  } else {
    lcdPrint(LCD_STATUS_ROW, LCD_STATUS_COL, LCD_RUNNING);
  }
}

int main(void) {
  uint8_t subLoops = ZERO;
  uint8_t loops = ZERO;
  unsigned char button;

  setup();
  // setup buttons
  button = get_single_debounced_button_press(ANY_BUTTON);

  // print out counter
  serialPrompt(loops);
  sprintf(lcdOutBuf, LCD_LOOP_SPR, loops);
  lcdPrint(LCD_COUNT_ROW, LCD_COUNT_COL, lcdOutBuf);

  while (TRUE) {

    // check for commands
    if (serialRead()) {
      cmdDo();
      serialPrompt(loops);
    }
    delay_ms(LOOP_DELAY);

    // check for ping results
    if (pinged) {
      if (!pinging) {
	data.distance[id] = ticks[id];
	dataReady(READY_DISTANCES);
	if (id == SDMIO_MAIN) {
	  sprintf(lcdOutBuf, LCD_DISTANCE_SPR, data.distance[id]);
	  lcdPrint(LCD_DISTANCE_0_ROW, LCD_DISTANCE_0_COL, lcdOutBuf);
	}
	pinged = FALSE;
      }
    }

    // check buttons
    button = get_single_debounced_button_press(ANY_BUTTON);
    if (button & TOP_BUTTON)  {
      getCountF();
    }
    if (button & MIDDLE_BUTTON) {
      getCompassF();
    }
    if (button & BOTTOM_BUTTON) {
      sdmioPing(ZERO);
    }

    // manage lop counter
    subLoops++;
    if (subLoops == SUB_LOOP_MAX) {
      subLoops = ZERO;
      loops++;
      sprintf(lcdOutBuf, LCD_LOOP_SPR, loops);
      lcdPrint(LCD_COUNT_ROW, LCD_COUNT_COL, lcdOutBuf);
    }
  }
  return FALSE;
}
