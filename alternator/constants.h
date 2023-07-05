#ifndef ALTERNATOR_CONSTANTS_H
#define ALTERNATOR_CONSTANTS_H

#include <stdint.h>

// Число шагов ШИМ -- это TIM2_ARR
// Итоговая частота ШИМ: 16MHz / PWM_SIZE
#define PWM_SIZE  1000 // freq=16kHz

// Забъёмся на максимальный ШИМ в 60%
#define PWM_MAX (PWM_SIZE * 0.6)

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
//                10
//           измер-ий          Vdd     R4       R3      R4
#define UIN_MAX  (10 * (27.4 / 5.0) * (2000. / (10000 + 2000)) * 1024)

// Входное напряжение для максимального ШИМ
#define UIN_MIN  (10 * (27.0 / 5.0) * (2000. / (10000 + 2000)) * 1024)

// Коэффициент для пересчёта напряжения в ШИМ
#define PWM_SLOPE (PWM_MAX / (UIN_MAX - UIN_MIN));

// Число шагов PWM для индикаторного светодиода, в тиках
#define PWM_LED_MAX UINT16_MAX

#define PWM_LED_SLOPE (PWM_LED_MAX / (UIN_MAX - UIN_MIN))

#endif  // ALTERNATOR_CONSTANTS_H
