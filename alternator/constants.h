#ifndef ALTERNATOR_CONSTANTS_H
#define ALTERNATOR_CONSTANTS_H

#include <stdint.h>

// Число шагов ШИМ -- это TIM1_ARR
#define PWM_SIZE  UINT16_MAX
static const uint16_t kPWM_size = PWM_SIZE;

// Забъёмся на максимальный ШИМ в 60%
#define PWM_MAX (PWM_SIZE * 0.6)
static const uint16_t kPWM_max = PWM_MAX;

/**              R3         R4
 *   Vin  <----|====|--*--|====|----||| GND
 * 0..30V              |
 *                     |
 *                    Uin 
 *                   0..5V
 *                  0..1023 ticks
 * 
 * Vin = Uin * (R3 + R4) / R4
 * Uin = Vin * R4 / (R3 + R4)
 */

// Входное напряжение для нулевого ШИМ
//                 Vmax    Vdd      R4     R3      R4
#define Vmax 27.4
#define UIN_MAX  ((27.4 / 4.94) * (4.93 / 29.2) * 1024)
static const uint16_t kUin_max = UIN_MAX;

// Входное напряжение для максимального ШИМ
#define UIN_MIN  (UIN_MAX / Vmax * 27.0)
static const uint16_t kUin_min = UIN_MIN;

// Коэффициент для пересчёта напряжения в ШИМ
#define PWM_SLOPE (PWM_MAX / (UIN_MAX - UIN_MIN))
static const uint16_t kPWM_slope = PWM_SLOPE;

// Число шагов PWM для индикаторного светодиода, в тиках
#define PWM_LED_PSC 3
#define PWM_LED_FREQ (2000000. / (0b1 << PWM_LED_PSC))
#define PWM_LED_SIZE (PWM_LED_FREQ / 5)
static const uint16_t kPWM_LED_size = PWM_LED_SIZE;
#define PWM_LED_MAX (PWM_LED_SIZE * 1.0)
static const uint16_t kPWM_LED_max = PWM_LED_MAX;
#define PWM_LED_SLOPE (PWM_LED_MAX / (UIN_MAX - UIN_MIN))
static const uint16_t kPWM_LED_slope = PWM_LED_SLOPE;

#endif  // ALTERNATOR_CONSTANTS_H
