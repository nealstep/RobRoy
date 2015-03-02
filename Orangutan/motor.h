#ifndef MOTOR_H
#define MOTOR_H

// main includes
#include "base.h"

// constants
#define MOTOR_COUNT 2
#define MOTOR_COUNT_INIT 0
#define MOTOR_INIT 0
#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1

// variables
extern int countLeft, countRight;
extern int motorLeft, motorRight;

//functions
void motorSetup(void);
void motorGetCounts(void);
void motorSetSpeeds(int speedLeft, int speedRight);

#endif // MOTOR_H
