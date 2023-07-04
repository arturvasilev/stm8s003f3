#ifndef ALTERNATOR_TIMERS_H
#define ALTERNATOR_TIMERS_H

#include "stdint.h"

typedef struct Timer {
  uint16_t period;
  uint16_t last_call;
  void (*cb)(void*);
  void *arg;

  struct Timer *prev;
  struct Timer *next;
} timer_t;

typedef struct SWPWM {
  uint16_t period;
  uint16_t duty_cycle;
  uint16_t counter;
  void (*cb_on)(void);
  void (*cb_off)(void);

  struct SWPWM *prev;
  struct SWPWM *next;
} swPWM_t;

void Timers_tick();

uint16_t Timers_now();

void Timers_add(timer_t *new);
void Timers_remove(timer_t *rm);

void swPWM_add(swPWM_t *new);
void swPWM_remove(swPWM_t *rm);

#endif  // ALTERNATOR_TIMERS_H
