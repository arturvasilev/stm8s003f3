#include "utils.h"
#include "init.h"

#include "../defines.h"
#include "../isr_handlers.h"
#include "../regmap.h"

#include <stdint.h>

void main(void) {

  disableInterrupts();

  init();

  enableInterrupts();

  for (;;) {
    wfi();
    PWM_update();
  }
}
