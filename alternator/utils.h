#ifndef ALTERNATOR_UTILS_H
#define ALTERNATOR_UTILS_H

#include <stdint.h>

struct ADC_t {
  uint8_t channels[2];
  uint8_t converting_idx;
  uint16_t vals[2];
};
extern struct ADC_t ADC;

uint8_t next_ADC_channel();

uint16_t ADC_read();

struct EEPROM_t {
  uint16_t PWM_max;
};
volatile struct EEPROM_t __at(0x4000) EEPROM;
// assert не работает, поэтому проверять руками
// static_assert(sizeof(EEPROM) <= 128);
extern struct EEPROM_t EEPROM_RAM;

void EEPROM_load();
void EEPROM_save();

void PWM_set(uint16_t pwm);
uint16_t PWM_get();

#endif  // ALTERNATOR_UTILS_H
