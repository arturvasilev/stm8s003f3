#include "defines.h"
#include "regmap.h"

void GPIO_init() {
}

void main(void) {
  // Disable unnecessary peripherals

  enableInterrupts();

  for (;;) {
    wfi();
  }
}
