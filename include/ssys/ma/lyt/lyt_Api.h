#pragma once

#include "decomp.h"

namespace sead {
class Heap;
} // namespace sead

namespace ssys {
namespace ma {
class Allocator;

namespace lyt {
// the allocators of the layouts (names after libgarden)
Allocator* GetAllocator(); // 0x0056A838 | libgarden [tier A]
Allocator* GetDeviceAllocator(); // 0x0056A8AC
// an ExpHeap "2D_UI_Heap" of size in parent with locking, and both allocators on it (name is ours)
void InitializeHeap(u32 size, sead::Heap* parent); // 0x0056A8BC

extern Allocator* s_pAllocator;
extern Allocator* s_pDeviceAllocator;
} // namespace lyt
} // namespace ma
} // namespace ssys
