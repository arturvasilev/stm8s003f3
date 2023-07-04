#include "swPWM.h"

#include "constants.h"

#include "../regmap.h"

#include <stdbool.h>

// PWM для индикаторного светодиода,
// Который должен повторять скважность
// полезного сигнала, но с частотой 2Гц
swPWM_t LED_PWM;

void LED_PWM_on();
void LED_PWM_off();

void swPWM_init() {
  // Начнём с постоянно горящим диодом
  LED_PWM.period = LED_PWM.duty_cycle = SWPWM_LED_MAX;
  LED_PWM.counter = 0;
  LED_PWM.cb_on = LED_PWM_on;
  LED_PWM.cb_off = LED_PWM_off;

  {
    // PD3 в Push-pull output
    const uint8_t mask = 0b1 << 3;
    GPIOD.DDR |= mask;
    GPIOD.CR1 |= mask;
  }

  swPWM_add(&LED_PWM);
}

void swPWM_set(uint16_t pwm) {
  const bool kPrevState = LED_PWM.counter < LED_PWM.duty_cycle;

  LED_PWM.duty_cycle = pwm;

  const bool kNewState = LED_PWM.counter < LED_PWM.duty_cycle;

  // Проверим, не изменилось ли состояние после изменения ШИМ
  if (kPrevState && !kNewState) {
    // Надо выключить
    LED_PWM.cb_off();
  } else if (!kPrevState && kNewState) {
    // Сменилось с Выкл на Вкл
    LED_PWM.cb_on();
  }
}

void LED_PWM_on() {
  GPIOD.ODR |= 0b1 << 3;
}

void LED_PWM_off() {
  GPIOD.ODR &= (0b1 << 3) ^ 0xff;
}
