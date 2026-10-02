#pragma once

// nn::nstd: C string functions of the nn library (C interface; the name is from the binary)

#include "decomp.h"

extern "C" void* nnnstdMemCpy(void* destination, const void* source, size_t size); // 0x0012EC68
