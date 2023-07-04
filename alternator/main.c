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

  disableInterrupts();

  init();

  enableInterrupts();

  for (;;) {
    wfi();
    PWM_update();
  }
}
