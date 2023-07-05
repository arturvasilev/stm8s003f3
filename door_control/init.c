#ifndef DOOR_CONTROL_INIT_C
#define DOOR_CONTROL_INIT_C

#include "constants.h"
#include "utils.h"

#include "../defines.h"
#include "../regmap.h"

void clock_init() {
  // Отключим ненужную периферию
  CLK_PCKENR1.I2C = false;
  CLK_PCKENR1.SPI = false;
  CLK_PCKENR1.UART1234 = false;
  CLK_PCKENR1.TIM46 = false;
  CLK_PCKENR1.TIM3 = false;
  CLK_PCKENR1.TIM1 = false;

  CLK_PCKENR2.ADC = false;
  CLK_PCKENR2.AWU = false;

  // Будем использовать Low-speed
  // Это 128кГц

  // Включим low-speed osc
  CLK_ICKR.LSIEN = true;

  // Подождём стабилизации LSI
  while (!CLK_ICKR.LSIRDY) nop();

  // Переключаем fmaster на LSI
  CLK_SWR = enCLK_CMSR_CKM_LSI;
  CLK_SWCR.SWEN = true; // Trigger fmaster clock switching
  
  // Подождём переключения на LSI
  while (CLK_CMSR.CKM != enCLK_CMSR_CKM_LSI) nop();

  // Выключим HSI
  CLK_ICKR.HSIEN = false;

  // Подождём переключения fmaster
  while(CLK_SWCR.SWBSY) nop();

  // Здесь у нас fmaster = fCPU = 128kHz
}

void GPIO_init() {
  // PA1 io -- Open door
  // PA2 io -- Close door
  // PA3 i  -- Bout -- button out
  // PB4 i  -- Bin -- button inside
  // PD3 для TIM2_CH2

  // PA1, PA2 настроим на output
  {
    static const uint8_t mask = 0b1 << 1 | 0b1 << 2;
    GPIOA.DDR |= mask;
    GPIOA.CR1 |= mask;
  }

  // PA3 на input
  {
    static const uint8_t mask = 0b1 << 3;
    GPIOA.CR1 |= mask;  // Input with pull-up
    GPIOA.CR2 |= mask;  // External interrupt enable
  }

  // PB4 на input
  {
    static const uint8_t mask = 0b1 << 4;
    GPIOB.CR1 |= mask;  // Input with pull-up
    GPIOB.CR2 |= mask;  // External interrupt enable
  }
  
  // PD3 для TIM2_CH2
  {
    static const uint8_t mask = 0b1 << 3;
    // PD4 as output
    GPIOD.DDR |= mask;
    // Push-pull
    GPIOD.CR1 |= mask;
  }
}

void TIM2_init() {
  // Вывод PWM с частотой 1Hz
  // на индикаторный LED
  // 100% -- door open
  //   5% -- door closed
  // Вывод -- на PD3 (TIM2_CH2)

  // Preload enable -- amust for PWM
  TIM2_CCMR2.OCPE = true;
  
  // Режим работы канала
  TIM2_CCMR2.OCM = TIMx_CCMR_OCM_PWM_mode1;

  // Compare output enable
  TIM2_CCER1.CC2E = true;

  // Auto-reload buffer
  TIM2_CR1.ARPE = true;

  // Установим частоту
  // Сейчас f_lsi = 128kHz
  // prescaler -- power of 2
  TIM2_PSCR.PSC = 7;  // 128kHz / 2^7 = 1kHz
  const uint16_t kPWM_size = PWM_SIZE;
  TIM2_ARRH = kPWM_size >> 8;
  TIM2_ARRL = kPWM_size & 0xff;

  // Сразу зажигаем светодиод
  LED_Bright();

  // Включаем сконфигурированный таймер
  TIM2_CR1.CEN = true;
}

void init() {
  clock_init();
  GPIO_init();
  TIM2_init();
}

#endif  // DOOR_CONTROL_INIT_C

