#pragma once

#include "decomp.h"

namespace ssys {
namespace ma {
class VramHeap;
} // namespace ma
} // namespace ssys

namespace graphic {
// sets the current heaps of the MemoryMgr for its lifetime (NULL: keep the heap) and puts the previous ones back
// (member names are ours)
// RTTI N7graphic13CurrentSetterE @ 0x008D3F18
// vtable 0x0090BCD4 (vptr 0x0090BCDC), offset_to_top 0, 2 entries
class CurrentSetter
{
public:
    CurrentSetter(ssys::ma::VramHeap* heap0, ssys::ma::VramHeap* heap1, ssys::ma::VramHeap* heap2); // 0x00134080
    virtual ~CurrentSetter(); // 0x001340F8 slot 0x00
    // 0x006179D0 (deleting dtor)

    ssys::ma::VramHeap* m_pPrevHeaps[3]; // 0x4
};
ASSERT_SIZE(CurrentSetter, 0x10);
} // namespace graphic
