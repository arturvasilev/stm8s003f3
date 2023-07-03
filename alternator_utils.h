#ifndef ALTERNATOR_UTILS_H
#define ALTERNATOR_UTILS_H

#include <stdint.h>

#include "regmap.h"

struct ADC_t {
  uint8_t channels[2];
  uint8_t converting_idx;
  uint16_t vals[2];
} ADC;

uint8_t next_ADC_channel() {
  ++ADC.converting_idx;
  ADC.converting_idx %= sizeof(ADC.channels);
  return ADC.converting_idx;
}

uint16_t ADC_read() {
  uint16_t ret = 0;

  for (uint8_t i = 0; i < 10; ++i)
    ret += ADC_DBxR[i].high << 8 + ADC_DBxR[i].low;

  return ret / 10;
}

#endif  // ALTERNATOR_UTILS_H
