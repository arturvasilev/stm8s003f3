#include "utils.h"
#include "init.h"

#include "../defines.h"
#include "../isr_handlers.h"
#include "../regmap.h"

#include <stdint.h>

void LED_on() {
  // PD3 в Push-pull в высокий уровень
  const uint8_t mask = 0b1 << 3;
  GPIOD.DDR |= mask;
  GPIOD.CR1 |= mask;
  GPIOD.ODR |= mask;
}

void main(void) {

  disableInterrupts();

  init();

  // Индикаторный светодиод, что всё прошилось и заработало
  LED_on();

  enableInterrupts();

  for (;;) {
    wfi();
    PWM_update();
  }
}
