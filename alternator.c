#include "defines.h"
#include "regmap.h"

volatile struct {

} __at(0x4000) EEPROM;
static_assert(sizeof(EEPROM) <= 128);

void GPIO_init() {
}

void main(void) {
  // Disable unnecessary peripherals

  enableInterrupts();

  for (;;) {
    wfi();
  }
}
