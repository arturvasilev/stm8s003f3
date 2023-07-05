#ifndef DOOR_CONTROL_UTILS_H
#define DOOR_CONTROL_UTILS_H

#include "constants.h"

#include <stdint.h>

void LED_Bright() {
  const uint16_t kPWM = PWM_BRIGHT;

  TIM2_CCR2H = kPWM >> 8;
  TIM2_CCR2L = kPWM & 0xff;
}

void LED_Blink() {
  const uint16_t kPWM = PWM_Blink;

  TIM2_CCR2H = kPWM >> 8;
  TIM2_CCR2L = kPWM & 0xff;
}

#endif  // DOOR_CONTROL_UTILS_H
