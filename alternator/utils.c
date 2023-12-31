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
   ADC.val_buffer += ADC_read();

  ++ADC.counter;

  if (ADC.counter >= MEAS_ACCUMULATE) {
    // Сбросим число измерений одного датчика
    ADC.counter = 0;

    // Опубликуем сделанное измерение
    ADC.vals[ADC.converting_idx] = ADC.val_buffer;
    ADC.val_buffer = 0;

    // Переключимся на следующий канал
    ADC_CSR.CH = ADC_next_channel();

    ADC.dirty = true;
  }
}

void PWM_update() {
  // Проверим, что это свежее измерение -- его counter=0
  // В противном случае, если измерения ещё накапливаются,
  // нам нечего менять
  if (ADC.dirty == false) return;

  ADC.dirty = false;

  if (ADC.dirty) PA1_on();
  else PA1_off();

  // Обновим ШИМ до транзисторов
  {
    uint16_t pwm;

    if (*Vin < kUin_min) pwm = kPWM_max;
    else if (*Vin >= kUin_max) pwm = 0;
    else {
      pwm = (kUin_max - *Vin) * kPWM_slope;
    }

    TIM1_CCR4H = pwm >> 8;
    TIM1_CCR4L = pwm & 0xff;
  }

  // Обновим ШИМ до индикаторного светодиода
  {
   uint16_t pwm;

    if (*Vin < kUin_min) pwm = kPWM_LED_max;
    else if (*Vin >= kUin_max) pwm = 0;
    else {
      pwm = (kUin_max - *Vin) * kPWM_LED_slope;
    }

    TIM2_CCR1H = pwm >> 8;
    TIM2_CCR1L = pwm & 0xff;
  }
}

void PA1_on() {
  static const uint8_t mask = 0b1 << 1;
  GPIOA.DDR |= mask;
  GPIOA.ODR |= mask;
  GPIOA.CR1 |= mask;
}

void PA1_off() {
  GPIOA.ODR &= (0b1 << 1) ^ 0xff;
}

#endif  // ALTERNATOR_UTILS_C
