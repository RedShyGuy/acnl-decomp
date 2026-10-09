#pragma once

#include "decomp.h"

namespace nn {
namespace init {
// the heap of malloc / free: a memory block of size bytes
void InitializeAllocator(size_t size); // 0x0011D56C | mk7dlp:bytes [tier B]
// the heap of malloc / free at address
void InitializeAllocator(uptr address, size_t size); // 0x0011E4DC | tier C
} // namespace init
} // namespace nn
