#include "serial.h"

char serialInBuf[SERIAL_BUFFER_SIZE];
uint8_t serialPos;
char serialOutBuf[SERIAL_BUFFER_SIZE];
char cmdBuf[SERIAL_BUFFER_SIZE];
uint8_t cmdPos;

void serialSetup(void) {
  serialPos = 0;
  cmdPos = 0;
  serial_set_baud_rate(UART1, BAUD);
  serial_receive_ring(UART1, serialInBuf, SERIAL_BUFFER_SIZE);
  // use interupts to read serial
  serial_set_mode(UART1, SERIAL_AUTOMATIC);
  serial_send_blocking(UART1, LF, strlen(LF));
}

void serialPrint(char *str) {
  serial_send_blocking(UART1, str, strlen(str));
  serial_send_blocking(UART1, LF, strlen(LF));
}

uint8_t serialRead(void) {
  uint8_t pos;
  char c;
  
  // just consume buffer
  pos = serial_get_received_bytes(UART1);
  while (serialPos != pos) {
    c = serialInBuf[serialPos++];
    cmdBuf[cmdPos] = c;
    // if we hit new line finish command
    if (c == '\n') {
      cmdBuf[cmdPos] = 0;
      cmdPos = 0;
      return TRUE;
    }
    cmdPos++;
  }
  return FALSE;
}

void serialInterrupt(void) {
  return;
}
