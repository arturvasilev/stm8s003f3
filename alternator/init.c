#ifndef ALTERNATOR_INIT_C
#define ALTERNATOR_INIT_C

#include "init.h"
#include "utils.h"
#include "constants.h"
#include "swPWM.h"

#include "../regmap.h"
#include "../defines.h"

#include <stdint.h>

void clock_init() {
  // Disable clock for unnecessary peripherals
  CLK_PCKENR1.I2C = false;
  CLK_PCKENR1.TIM46 = false;
  CLK_PCKENR1.TIM3 = false;
  CLK_PCKENR1.TIM1 = false;

  // Повысим частоту от HSI
  CLK_CKDIVR.HSIDIV = CLK_CKDIVR_HSIDIV_fhsi;
  // И понизим для CPU
  CLK_CKDIVR.CPUDIV = CLK_CKDIVR_CPUDIV_fmaster_8;

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

  // Preload enable -- amust for PWM
  TIM2_CCMR1.OCPE = true;
  // Режим работы канала
  TIM2_CCMR1.OCM = TIMx_CCMR_OCM_PWM_mode1;
  // TIM2_CCMR1.OCM = 0b11;
  // Compare output enable
  TIM2_CCER1.CC1E = true;
  // Auto-reload buffer
  TIM2_CR1.ARPE = true;

  // Установим частоту
  // Сейчас f_hsi = 16MHz
  TIM2_PSCR.PSC = 0;
  const uint16_t kPWM_size = PWM_SIZE;
  TIM2_ARRH = kPWM_size >> 8;
  TIM2_ARRL = kPWM_size & 0xff;

  // Включаем сконфигурированный таймер
  TIM2_CR1.CEN = true;
}

void TIM4_init() {
  // Настроим простейший внутренний таймер на 1кГц
  // Для работы software-timers

  // Enable preload of auto-reload
  TIM4_CR1.ARPE = true;
  // Включим update interrupt
  TIM4_IER.UIE = true;

  // fmaster = 16MHz
  TIM4_PSCR.PSC = 7;  // = 2^7 = 128

  // Здесь частота =125кГц
  // Поэтому ставим ARR = 125, и получим частоту прерывай
  // по переполнению 1 кГц!
  TIM4_ARR = 125;

  // Запуск таймера
  TIM4_CR1.CEN = true;
}

void ADC_init() {
  // PC4=AIN2 -- Uin
  // PD2=AIN3 -- Uout
  ADC.channels[0] = 2;
  ADC.channels[1] = 3;
  ADC.vals[0] = ADC.vals[1] = 0;

  // Disable Shmidtt triggers -- high means "disable"
  // Lowers static power consumption on I/O port
  ADC_TDRL |= 0b1 << 3 || 0b1 << 2;

  // Будем использовать continious buffered режим
  // После каждого EOC -- останавливаемся, меняем канал,
  // и запускаем дальше
  // Считывание 10 измерений за раз

  // fADC = fmaster / 8  ==>  2MHz
  ADC_CR1.SPSEL = ADC_CR1_SPSEL_fmaster_8;
  // ADC_sample_rate = fADC / 14 / 10 ==>  14.2kSPS
  // И нам надо два канала -- это 7.1kSPS на канал ==> T_sample = 140us

  // Включим прерывание по окончании конвертации
  ADC_CSR.EOCIE = true;
  // Включаем непрерывную конвертацию
  ADC_CR1.CONT = true;
  // Делаем за одно прерывание 10 измерений,
  // Складывая результаты в ADC_DBxR
  ADC_CR3.DBUF = true;

  // Начнём конвертации с Uin
  ADC_CSR.CH = ADC.channels[ADC.converting_idx];

  // Выведем ADC из энергосебергающего режима
  ADC_CR1.ADON = true;

  // Ждём 7us до его пробуждения
  // У нас fCPU = 2MHz
  for (volatile uint32_t i = 0; i < (7 * 2); ++i)
    ;

  // Пинаем ADC для начала конвертаций
  ADC_CR1.ADON = true;
}

void init() {
  clock_init();
  GPIO_init();
  TIM2_init();
  TIM4_init();
  ADC_init();
  swPWM_init();
}

#endif  // ALTERNATOR_INIT_C
