#include "motor.h"

int countLeft, countRight;
int motorLeft, motorRight;


void motorSetup(void) {
  motorSetSpeeds(MOTOR_INIT, MOTOR_INIT);
}

void motorGetCounts(void) {
  countLeft = svp_get_counts_and_reset_ab();
  countRight = svp_get_counts_and_reset_cd();
}

void motorSetSpeeds(int speedLeft, int speedRight) {
  motorLeft = speedLeft;
  motorRight = speedRight;
  set_m1_speed(motorLeft);
  set_m2_speed(motorRight);
  svp_get_counts_and_reset_ab();
  svp_get_counts_and_reset_cd();
  countLeft = MOTOR_COUNT_INIT;
  countRight = MOTOR_COUNT_INIT;
}
