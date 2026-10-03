#pragma once

// nn::nstd: C string functions of the nn library (C interface; the name is from the binary)

#include "decomp.h"

extern "C" void* nnnstdMemCpy(void* destination, const void* source, size_t size); // 0x0012EC68
// snprintf of the nn library (the name is from the reference symbols)
extern "C" int nnnstdTSNPrintf(char* buffer, size_t size, const char* format, ...); // 0x007B2B44 | fefates:callgraph
