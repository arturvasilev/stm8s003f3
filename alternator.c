#include "defines.h"
#include "regmap.h"
#include "isr_alternator_handlers.h"

volatile struct {
  uint8_t __dummy;
} __at(0x4000) EEPROM;
// assert не работает, поэтому проверять руками
// static_assert(sizeof(EEPROM) <= 128);

void init();

void main(void) {
  init();

  enableInterrupts();

  for (;;) {
    wfi();
  }
}

void clock_init() {
  // Disable clock for unnecessary peripherals
  CLK_PCKENR1.I2C = false;
  CLK_PCKENR1.TIM46 = false;
  CLK_PCKENR1.TIM3 = false;
  CLK_PCKENR1.TIM1 = false;

  // Повысим частоту от HSI
  CLK_CKDIVR.HSIDIV = fhsi_4;
  // И понизим для CPU
  CLK_CKDIVR.CPUDIV = fmaster_2;

  // Подождём переключения clock
  while (CLK_SWCR.SWBSY) nop();
}

void GPIO_init() {
  // PD4 для вывода PWM TIM2_CH2
  {
    static const uint8_t mask = 0b1 << 4;
    // PD4 as output
    GPIOD.DDR |= mask;
    // Push-pull
    GPIOD.CR1 |= mask;
    // Output speed up to 10MHz
    GPIOD.CR2 |= mask;
  }

  // Для ADC остальные порты (PC4, PD2) здесь не трогаем
}

void TIM2_init() {
  // Вывод PWM с частотой 500кГц
  // 16-bit ШИМ регулирует выходную мощность
  // Вывод -- на PD4 (TIM2_CH2)
  
  // Preload enable
  TIM2_CCMR2.OCPE = 1;
  // Режим работы канала
  TIM2_CCMR2.OCM = PWM_mode1;
  // Compare output enable
  TIM2_CCER1.CC2E = 1;

  // Установим частоту
  // Сейчас f_hsi = 4MHz
  // Нам надо 500кГц -- это /8
  TIM2_PSCR = 3;  // 2^3 = 8

  // Включаем сконфигурированный таймер
  TIM2_CR1.CEN = 1;
}

void PWM_set(uint16_t counter) {
  TIM2_CCR2H = counter >> 8;
  TIM2_CCR2L = counter & 0xff;
}

void init() {
  clock_init();
  GPIO_init();
  TIM2_init();
}
