#include "schedule.h"

void (*scheduleFunc)(void);

// schedule overflow - when jobs run
ISR(TIMER3_OVF_vect) {
  (*scheduleFunc)();
}

void scheduleSetup(void (*f)(void)) {
  scheduleFunc = f;

  // setup timer ~256hz
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    TCNT3H = SCHEDULE_256HZ_H;
    TCNT3L = SCHEDULE_256HZ_L;
  }
  TCCR3B = _BV(CS00);
  TIMSK3 = _BV(TOIE3);
}
