#include "timers.h"

uint16_t now = 0;
timer_t *timers = 0;
swPWM_t *pwms = 0;

// Возвращает время следующего срабатывания
uint16_t Timers_next_call(timer_t *t) {
  if (UINT16_MAX - t->last_call > t->period) return t->last_call + t->period;

  return t->period - (UINT16_MAX - t->last_call);
}

void Timers_tick() {
  ++now;

  for (timer_t *tim = timers; tim != 0; tim = tim->next) {
    // Process timer
    if (Timers_next_call(tim) <= now && tim->cb != 0) tim->cb(tim->arg);
  }

  for (swPWM_t *pwm = pwms; pwm != 0; pwm = pwm->next) {
    if (pwm->counter == 0 && pwm->cb_on)
      pwm->cb_on();
    else if (pwm->counter == pwm->duty_cycle && pwm->cb_off)
      pwm->cb_off();

    ++pwm->counter;

    pwm->counter %= pwm->period;
  }
}

uint16_t Timers_now() { return now; }

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

void swPWM_add(swPWM_t *new) {
  if (pwms != 0) {
    new->next = pwms;
    pwms->prev = new;
    new->prev = 0;
    pwms = new;
  } else {
    pwms = new;
    new->prev = new->next = 0;
  }
}

void swPWM_remove(swPWM_t *rm) {
  for (swPWM_t *pwm = pwms; pwm != 0; pwm = pwm->next) {
    if (pwm == rm) {
      pwm->prev->next = pwm->next;
      pwm->next->prev = pwm->prev;
      return;
    }
  }
}
