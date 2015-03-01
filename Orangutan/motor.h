#ifndef MOTOR_H
#define MOTOR_H

// main includes
#include "base.h"

// variables
extern int countLeft, countRight;
extern int motorLeft, motorRight;

//functions
void motorSetup(void);
void motorGetCounts(void);
void motorSetSpeeds(int speedLeft, int speedRight);

#endif // MOTOR_H
