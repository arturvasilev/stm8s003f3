#ifndef ALTERNATOR_UTILS_H
#define ALTERNATOR_UTILS_H

#include <stdint.h>

struct ADC_t {
  uint8_t channels[2];
  uint8_t converting_idx;
  uint16_t vals[2];
};
extern struct ADC_t ADC;

uint8_t next_ADC_channel();

uint16_t ADC_read();

#endif  // ALTERNATOR_UTILS_H
