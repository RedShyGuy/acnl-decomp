#pragma once

#include "decomp.h"

namespace sead {
class ExpHeap;
class Heap;
} // namespace sead

// helpers of the game for its heaps (names are ours; the functions are not decompiled yet)
namespace HeapUtil {
// an ExpHeap of size in parent named name (the last argument is not used)
sead::ExpHeap* CreateExpHeapWithCName(u32 size, sead::Heap* parent, const char* name, s32 unused); // 0x0011EE4C
// adjusts heap if there is one (the freed size, else 0)
u32 AdjustHeap(sead::Heap* heap); // 0x0011D918
} // namespace HeapUtil
