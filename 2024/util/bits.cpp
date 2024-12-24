#include "bits.h"

uint32_t bitmask(uint32_t n) {
  return (n == 0) ? 0 : (1U << n) - 1;
}

uint32_t bitmask(int32_t n) {
  return (n == 0) ? 0 : (1U << n) - 1;
}

uint64_t bitmask(uint64_t n) {
  return (n == 0) ? 0 : (1ULL << n) - 1;
}