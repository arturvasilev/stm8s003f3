#ifndef ALTERNATOR_UTILS_C
#define ALTERNATOR_UTILS_C

#include "utils.h"

#include "../regmap.h"

struct ADC_t ADC;
struct EEPROM_t EEPROM_RAM; 

uint8_t next_ADC_channel() {
  ++ADC.converting_idx;
  ADC.converting_idx %= sizeof(ADC.channels);
  return ADC.converting_idx;
}

uint16_t ADC_read() {
  uint16_t ret = 0;

  for (uint8_t i = 0; i < 10; ++i)
    ret += ADC_DBxR[i].high << 8 + ADC_DBxR[i].low;

  return ret / 10;
}

void EEPROM_load() {
  EEPROM_RAM = EEPROM;
}

void EEPROM_save() {
  EEPROM = EEPROM_RAM;
}

void PWM_set(uint16_t pwm) {
  // pwm = pwm > EEPROM_RAM.PWM_max ? EEPROM_RAM.PWM_max : pwm;
  uint16_t pwm_max = TIM2_ARRH << 8;
  pwm_max |= TIM2_ARRL;
  pwm %= pwm_max;

  TIM2_CCR1H = pwm >> 8;
  TIM2_CCR1L = pwm & 0xff;
}

uint16_t PWM_get() {
  uint16_t ret = TIM2_CCR1H;
  ret <<= 8;
  ret |= TIM2_CCR1L;
  return ret;
}

#endif  // ALTERNATOR_UTILS_C
