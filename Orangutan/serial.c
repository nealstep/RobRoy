#include "serial.h"

char serialInBuf[SERIAL_BUFFER_SIZE];
uint8_t serialPos;
char serialOutBuf[SERIAL_BUFFER_SIZE];
char cmdBuf[SERIAL_BUFFER_SIZE];
uint8_t cmdPos;

void serialSetup(void) {
  serialPos = ZERO;
  cmdPos = ZERO;
  serial_set_baud_rate(UART1, SERIAL_BAUD);
  serial_receive_ring(UART1, serialInBuf, SERIAL_BUFFER_SIZE);
  // use interupts to read serial
  serial_set_mode(UART1, SERIAL_AUTOMATIC);
  serial_send_blocking(UART1, LF, strlen(LF));
  // setup interrupt pin
  DDRB |= SERIAL_INTERRUPT;
  PORTB &= ~SERIAL_INTERRUPT;
}

void serialPrint(char *message) {
  serial_send_blocking(UART1, message, strlen(message));
  serial_send_blocking(UART1, LF, strlen(LF));
}

void serialPrompt(uint8_t loops) {
  sprintf(serialOutBuf, SERIAL_PROMPT_SPR, loops);
  serial_send_blocking(UART1, serialOutBuf, strlen(serialOutBuf));
}

uint8_t serialRead(void) {
  uint8_t pos;
  char c;
  
  // just consume buffer
  pos = serial_get_received_bytes(UART1);
  while (serialPos != pos) {
    c = serialInBuf[serialPos++];
    // loop when we reach end of buffer
    if (serialPos == SERIAL_BUFFER_SIZE) {
      serialPos = 0;
    }
    cmdBuf[cmdPos] = c;
    // if we hit new line finish command
    if (c == '\n') {
      cmdBuf[cmdPos] = STRING_TERMINATOR;
      cmdPos = ZERO;
      return TRUE;
    }
    cmdPos++;
  }
  return FALSE;
}

// toggle serial interupt line
void serialInterrupt(void) {
  PORTB ^= SERIAL_INTERRUPT;
}
