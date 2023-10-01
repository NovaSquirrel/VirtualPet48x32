#include "vpet.h"
#include <stdint.h>

#ifdef PLATFORM_PICO

void RandomSeed() {
	srandom(1337);
}

uint32_t RandomRaw() {
	return random();
}

uint32_t Random(uint32_t Bound) {
	return random() % Bound;
}

uint32_t RandomMinMax(uint32_t Min, uint32_t Max) {
	return random() % (Max-Min)+Min;
}
#else

#include "pcg_variants.h"
#include <time.h>

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

#endif
