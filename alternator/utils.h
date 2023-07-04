#ifndef ALTERNATOR_UTILS_H
#define ALTERNATOR_UTILS_H

#include <stdint.h>

struct ADC_t {
  // PC4=AIN2 -- Uin
  // PD2=AIN3 -- Uout
  uint8_t channels[2];
  uint8_t converting_idx;
  uint16_t vals[2];
};
extern struct ADC_t ADC;

void ADC_process();
void PWM_update();

extern uint16_t *Vin;
extern uint16_t *Vout;

#endif  // ALTERNATOR_UTILS_H
