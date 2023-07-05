#ifndef DOOR_CONTROL_CONSTANTS_H
#define DOOR_CONTROL_CONSTANTS_H

#define PWM_SIZE 1000

// 100% при открывании дверей
#define PWM_BRIGHT (1. * PWM_SIZE)
// 5% от ШИМ для моргания при закрытых дверях
#define PWM_Blink (0.05 * PWM_SIZE)

#endif  // DOOR_CONTROL_CONSTANTS_H
