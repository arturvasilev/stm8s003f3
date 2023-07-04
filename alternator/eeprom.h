#ifndef ALTERNATOR_EEPROM_H
#define ALTERNATOR_EEPROM_H

#include <stdint.h>

struct EEPROM_t {
  uint8_t _dummy;
};
volatile struct EEPROM_t __at(0x4000) EEPROM;
// assert не работает, поэтому проверять руками
// static_assert(sizeof(EEPROM) <= 128);
extern struct EEPROM_t EEPROM_RAM;

void EEPROM_load();
void EEPROM_save();

#endif  // ALTERNATOR_EEPROM_H
