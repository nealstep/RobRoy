#include "sdmio.h"

const uint8_t pins[SDMIO_NUM_PINS] = { _BV(PA3) };
#define INT_MASK _BV(PCINT3)

volatile uint8_t pinging;
volatile uint8_t pinged;
volatile uint8_t ticks[SDMIO_NUM_PINS];
volatile uint8_t id;

ISR(TIMER0_OVF_vect) {
  if (pinging) {
    pinging = FALSE;
    ticks[id] = SDMIO_BAD;
  }
}

ISR(PCINT0_vect) {
  if (pinging) {
    if (!(PINA & pins[id])) {
      pinging = FALSE;
      // get elapsed time
      ticks[id] = TCNT0;
    }
  }
}

void sdmioSetup(void) {
  uint8_t i;

  id = ZERO;
  pinging = FALSE;
  pinged = FALSE;
  // setup US pins
  for (i=0;i<SDMIO_NUM_PINS;i++) {
    DDRA |= pins[i];
    PORTA |= pins[i];
    ticks[i] = SDMIO_BAD;
  }

  // setup timer 0
  TCCR0A = SDMIO_TIMER_CONTROL_INIT;
  TCCR0B = _BV(CS02) | _BV(CS00); // div 1024 prescaler
  TIMSK0 = _BV(TOIE0); // overflow interrupt
  TCNT0 = ZERO;

  // enable interrupt vector 0 and listen for pcint1
  PCICR |= _BV(PCIE0);
  PCMSK0 |= INT_MASK;
}

uint8_t sdmioPing(uint8_t pinId) {
  if (pinged) {
    return FALSE;
  }
  id = pinId;
  // clear counters
  ticks[id] = ZERO;
  // set pin to output and bring to high and wait
  DDRA |= pins[id];
  PORTA |= pins[id];
  delay_us(SDMIO_WARM_TIME);
  // set to low for 12us, bring back up and wait for input
  PORTA &= ~ pins[id];
  delay_us(SDMIO_SIGNAL_TIME);
  PORTA |= pins[id];
  // set for input and wait for interrupt on pin
  DDRA &= ~ pins[id];
  // reset timer
  TCNT0 = ZERO;
  // indicate we have pinged and awaiting a response (pinging)
  pinging = TRUE;
  pinged = TRUE;

  return TRUE;
}
