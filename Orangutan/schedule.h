#ifndef SCHEDULE_H
#define SCHEDULE_H

// main includes
#include "base.h"

// std includes
#include <util/atomic.h>

void scheduleSetup(void (*f)(void));

#endif // SCHEDULE_H
