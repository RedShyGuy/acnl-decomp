#pragma once

#include "decomp.h"

namespace sead {
class ExpHeap;
} // namespace sead

// the heaps of the game made at the start (HeapInit::Create*Heap; the variable names after the heap names; only the
// ones decompiled code uses)
namespace HeapInit {
// "ModelEffectResHeap" (set by HeapInit::CreateModelEffectResHeap)
extern sead::ExpHeap* s_pModelEffectResHeap;
} // namespace HeapInit
