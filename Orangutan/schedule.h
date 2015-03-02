#ifndef SCHEDULE_H
#define SCHEDULE_H

// main includes
#include "base.h"

// constants
#define SCHEDULE_256HZ_H 0
#define SCHEDULE_256HZ_L 0

// std includes
#include <util/atomic.h>

// functions
void scheduleSetup(void (*f)(void));

#endif // SCHEDULE_H
