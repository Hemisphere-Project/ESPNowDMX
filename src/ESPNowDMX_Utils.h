
#pragma once

#include "ESPNowDMX_Common.h"

// Lightweight compression interface (stub - raw copy)

static inline size_t compressData(const uint8_t* input, size_t inputLen, uint8_t* output, size_t maxOutputLen) {
  if (inputLen > maxOutputLen) return 0;
  memcpy(output, input, inputLen);
  return inputLen;
}

static inline size_t decompressData(const uint8_t* input, size_t inputLen, uint8_t* output, size_t maxOutputLen) {
  if (inputLen > maxOutputLen) return 0;
  memcpy(output, input, inputLen);
  return inputLen;
}
