#ifndef ALTERNATOR_UTILS_C
#define ALTERNATOR_UTILS_C

#include "utils.h"
#include "constants.h"

#include "../regmap.h"

struct ADC_t ADC;
uint16_t *Vin = &ADC.vals[0];
uint16_t *Vout = &ADC.vals[1];

uint8_t ADC_next_channel() {
  ++ADC.converting_idx;
  
  ADC.converting_idx %= sizeof(ADC.channels);

  return ADC.channels[ADC.converting_idx];
}

uint16_t ADC_read() {
  // Точность -- 10-bit
  uint16_t ret = ADC_DRH << 2;
  ret |= ADC_DRL & 0b11;
  return ret;
}

void ADC_process() {
  ADC.vals[ADC.converting_idx] = ADC_read();

  // Переключимся на следующий канал
  ADC_CSR.CH = ADC_next_channel();
}

void PWM_update() {
  // Обновим ШИМ до транзисторов
  {
    uint16_t pwm;

    if (*Vin < kUin_min) pwm = kPWM_max;
    else if (*Vin >= kUin_max) pwm = 0;
    else {
      pwm = (kUin_max - *Vin) * kPWM_slope;
    }

    TIM2_CCR1H = pwm >> 8;
    TIM2_CCR1L = pwm & 0xff;
  }

  // Обновим ШИМ до индикаторного светодиода
  {
   uint16_t pwm;

    if (*Vin < kUin_min) pwm = kPWM_LED_max;
    else if (*Vin >= kUin_max) pwm = 0;
    else {
      // pwm = PWM_SLOPE * (*Vin - kUin_min);
      pwm = *Vin;
      pwm -= kUin_min;
      pwm *= kPWM_LED_slope;
    }

    TIM1_CCR3H = pwm >> 8;
    TIM1_CCR3L = pwm & 0xff;
  }
}

void PD3_on() {
  static const uint8_t mask = 0b1 << 3;
  GPIOD.DDR |= mask;
  GPIOD.ODR |= mask;
  GPIOD.CR1 |= mask;
}

void PD3_off() {
  GPIOD.ODR &= (0b1 << 3) ^ 0xff;
}

#endif  // ALTERNATOR_UTILS_C
