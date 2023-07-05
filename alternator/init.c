#ifndef ALTERNATOR_INIT_C
#define ALTERNATOR_INIT_C

#include "init.h"
#include "utils.h"
#include "constants.h"

#include "../regmap.h"
#include "../defines.h"

#include <stdint.h>

void clock_init() {
  // Disable clock for unnecessary peripherals
  CLK_PCKENR1.I2C = false;
  CLK_PCKENR1.TIM3 = false;

  // Повысим частоту от HSI
  CLK_CKDIVR.HSIDIV = CLK_CKDIVR_HSIDIV_fhsi;
  // И понизим для CPU
  CLK_CKDIVR.CPUDIV = CLK_CKDIVR_CPUDIV_fmaster_8;

  // Подождём переключения master clock
  while (CLK_SWCR.SWBSY) nop();
}

void GPIO_init() {
  // PC4 для вывода PWM TIM1_CH4
  {
    static const uint8_t mask = 0b1 << 4;
    // PD4 as output
    GPIOC.DDR |= mask;
    // Push-pull
    GPIOC.CR1 |= mask;
    // Fast GPIO -- up to 10MHz
    GPIOC.CR2 |= mask;
  }

  // PD4 для вывода индикаторного LED
  // С помощью низкочастотного PWM 2Hz TIM1_CH1
  {
    static const uint8_t mask = 0b1 << 4;
    // PD4 as output
    GPIOD.DDR |= mask;
    // Push-pull
    GPIOD.CR1 |= mask;
  }

  // Для ADC остальные порты здесь не трогаем
}

void TIM1_init() {
  // Вывод PWM с частотой 500кГц
  // Настроим TIM1_CH4 (pin PC4)

  // Preload of auto-reload register
  TIM1_CR1.ARPE = true;

  // Preload enable -- amust for PWM
  TIM1_CCMR4.OCPE = true;
  
  // Установим режим таймера ШИМ
  TIM1_CCMR4.OCM = TIMx_CCMR_OCM_PWM_mode1;

  // Compare output enable
  TIM1_CCER2.CC4E = true;

  // Установим частоту
  // f_hsi = 2MHz
  // Нам надо 2Гц = 2Mhz/
  // Здесь не 2^PSC, а f_cnt_ck = f_hsi/PSCR[15:0]
  TIM1_PSCRH = 0;
  TIM1_PSCRL = 0;

  TIM1_ARRH = kPWM_size >> 8;
  TIM1_ARRL = kPWM_size & 0xff;
  // Здесь частота 2MHz / 4 = 500kHz;

  // Automatic output enable
  TIM1_BKR.AOE = true;

  // Включим сконфигурированный таймер
  TIM1_CR1.CEN = true;
}

void TIM2_init() {
  // Индикационный диод
  // 2Hz, PWM [0..100%], и совпадает с реальной
  // Вывод -- на PD4 (TIM2_CH1)

  // Preload enable -- amust for PWM
  TIM2_CCMR1.OCPE = true;
  
  // Режим работы канала
  TIM2_CCMR1.OCM = TIMx_CCMR_OCM_PWM_mode1;

  // Compare output enable
  TIM2_CCER1.CC1E = true;

  // Auto-reload buffer
  TIM2_CR1.ARPE = true;

  // Установим частоту
  // Сейчас f_hsi = 16MHz
  TIM2_PSCR.PSC = PWM_LED_PSC; // 2^15 = 32768
  TIM2_ARRH = kPWM_LED_size >> 8;
  TIM2_ARRL = kPWM_LED_size & 0xff;

  // Включаем сконфигурированный таймер
  TIM2_CR1.CEN = true;
}

void ADC_init() {
  // PD3=AIN4 -- Uin
  // PD2=AIN3 -- Uout
  ADC.channels[0] = 4;
  ADC.channels[1] = 3;
  ADC.vals[0] = ADC.vals[1] = 0;
  // Начнём конвертации с Uin
  ADC.converting_idx = 0;
  ADC_CSR.CH = ADC.channels[ADC.converting_idx];

  // Disable Shmidtt triggers -- high means "disable"
  // Lowers static power consumption on I/O port
  for (uint8_t ch = 0; ch < sizeof(ADC.channels); ++ch)
    ADC_TDRL |= 0b1 << ADC.channels[ch];

  // После каждого EOC -- останавливаемся, меняем канал,
  // и запускаем дальше

  // fADC = fmaster ==>  2MHz
  ADC_CR1.SPSEL = ADC_CR1_SPSEL_fmaster_8;
  // ADC_sample_rate = fADC / 14 = 142kSPS

  // Включим прерывание по окончании конвертации
  ADC_CSR.EOCIE = true;

  // Выведем ADC из энергосебергающего режима
  ADC_CR1.ADON = true;

  // Ждём 7us до его пробуждения
  // У нас fCPU = 2MHz
  for (volatile uint32_t i = 0; i < (7 * 4); ++i)
    nop();

  // Пинаем ADC для начала конвертаций
  ADC_CR1.ADON = true;
}

void init() {
  clock_init();
  GPIO_init();
  TIM1_init();
  TIM2_init();
  ADC_init();
}

#endif  // ALTERNATOR_INIT_C
