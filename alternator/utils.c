#ifndef ALTERNATOR_UTILS_C
#define ALTERNATOR_UTILS_C

#include "utils.h"
#include "constants.h"
#include "eeprom.h"

#include "../regmap.h"

struct ADC_t ADC;
uint16_t *Vin = &ADC.vals[0];
uint16_t *Vout = &ADC.vals[1];

uint8_t ADC_next_channel() {
  ++ADC.converting_idx;
  ADC.converting_idx %= sizeof(ADC.channels);
  return ADC.converting_idx;
}

uint16_t ADC_read() {
  uint16_t ret = 0;

  // Точность -- 10-bit
  // Поэтому в сумме <10240 < MAX_UINT16
  for (uint8_t i = 0; i < 10; ++i)
    ret += ADC_DBxR[i].high << 8 + ADC_DBxR[i].low;

  return ret;
}

void ADC_process() {
  ADC.vals[ADC.converting_idx] = ADC_read();

  // Переключимся на следующий канал
  ADC_CSR.CH = ADC_next_channel();
}

void PWM_update() {
  uint16_t pwm;

  if (*Vin < UIN_MIN) pwm = PWM_MAX;
  else if (*Vin >= UIN_MAX) pwm = 0;
  else {
    // pwm = PWM_SLOPE * (*Vin - UIN_MIN);
    pwm = *Vin;
    pwm -= UIN_MIN;
    pwm *= PWM_SLOPE;
  }

  TIM2_CCR1H = pwm >> 8;
  TIM2_CCR1L = pwm & 0xff;
}

#endif  // ALTERNATOR_UTILS_C
