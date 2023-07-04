#ifndef ALTERNATOR_EEPROM_C
#define ALTERNATOR_EEPROM_C

#include "eeprom.h"

struct EEPROM_t EEPROM_RAM; 

void EEPROM_load() {
  EEPROM_RAM = EEPROM;  
}

void EEPROM_save() {
  EEPROM = EEPROM_RAM;
}

#endif  // ALTERNATOR_EEPROM_C
