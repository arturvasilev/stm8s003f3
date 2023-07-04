#ifndef ALTERNATOR_TIMERS_C
#define ALTERNATOR_TIMERS_C

#include <stdint.h>

uint16_t now = 0;

typedef struct {
  uint16_t period;
  uint16_t last_call;
  void (*cb)(void*);
  void *arg;

  timer_t *prev;
  timer_t *next;
} timer_t;

// Возвращает время следующего срабатывания
uint16_t Timers_next_call(timer_t *t) {
  if (UINT16_MAX - t->last_call > t->period)
    return t->last_call + t->period;
  
  return t->period - (UINT16_MAX - t->last_call);
}

timer_t *timers = 0;

void Timers_tick() {
  ++now;

  for (timer_t *tim = timers; tim != 0; tim = tim->next) {
    // Process timer
    if (Timers_next_call(tim) <= now && tim->cb != 0)
      tim->cb(tim->arg);
  }
}

void Timers_now() {
  return now;
}

void Timers_add(timer_t *new) {
  if (timers != 0) {
    new->next = timers;
    timers->prev = new;
    new->prev = 0;
    timers = new;
  } else {
    timers = new;
    new->prev = new->next = 0;
  }
}

void Timers_remove(timer_t *rm) {
  for (timer_t *tim = timers; tim != 0; tim = tim->next) {
    if (tim == rm) {
      tim->prev->next = tim->next;
      tim->next->prev = tim->prev;
      return;
    }
  }
}

#endif  // ALTERNATOR_TIMERS_C
