#include "vpet.h"
#include "pcg_variants.h"
#include <time.h>
#include <stdint.h>

pcg32_random_t RandomState;

void RandomSeed() {
  pcg32_srandom_r(&RandomState, time(NULL), (intptr_t)&RandomState);
}

uint32_t RandomRaw() {
  return pcg32_random_r(&RandomState);
}

uint32_t Random(uint32_t Bound) {
  return pcg32_boundedrand_r(&RandomState, Bound);
}

uint32_t RandomMinMax(uint32_t Min, uint32_t Max) {
  return pcg32_boundedrand_r(&RandomState, Max-Min)+Min;
}
