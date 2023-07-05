#include "init.h"

#include "../defines.h"
#include "../isr_handlers.h"
#include "../regmap.h"

void main(void) {

  disableInterrupts();

  init();

  enableInterrupts();

  for (;;) {
    wfi();
  }
}
