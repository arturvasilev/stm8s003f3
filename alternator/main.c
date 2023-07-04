#include "utils.h"
#include "init.h"

#include "../defines.h"
#include "../isr_handlers.h"
#include "../regmap.h"

#include <stdint.h>

void PD3_on() {
  const uint8_t mask = 0b1 << 3;
  GPIOD.DDR |= mask;
  GPIOD.CR1 |= mask;
  GPIOD.ODR |= mask;
}

void main(void) {
  PD3_on();
  EEPROM_RAM.PWM_max = 6 * (UINT16_MAX / 10);
  EEPROM_save();

  disableInterrupts();

  init();

  enableInterrupts();

  uint16_t pwm = PWM_get();
  uint16_t prescaler = 0;

  for (;;) {
    if (prescaler == 0) {
      pwm = (pwm + 1) % 255;
      PWM_set(pwm);
      GPIOD.ODR ^= 0b1 << 3;
    }
    prescaler = (prescaler + 1) % 16;
    wfi();
  }
}
