#pragma once

#include "decomp.h"

// The global delete operators of sead (both look for the heap that holds the memory). The two copies are
// equal; 0x002F8958 is the one of delete[] (the deleting destructors call 0x002FF7F8; libgarden names
// 0x002F8958 "operator delete(void*, unsigned int)", probably folded with it there).
void operator delete[](void* p); // 0x002F8958
