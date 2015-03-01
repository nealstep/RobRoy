#include "motor.h"

int countLeft, countRight;
int motorLeft, motorRight;


void motorSetup(void) {
  motorSetSpeeds(0, 0);
}

void motorGetCounts(void) {
  countLeft = svp_get_counts_and_reset_ab();
  countRight = svp_get_counts_and_reset_cd();
}

void motorSetSpeeds(int speedLeft, int speedRight) {
  set_m1_speed(speedLeft);
  set_m2_speed(speedRight);
  svp_get_counts_and_reset_ab();
  svp_get_counts_and_reset_cd();
  countLeft = 0;
  countRight = 0;
}
