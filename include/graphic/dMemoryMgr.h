#pragma once

#include "decomp.h"
#include "sead/seadGfxMemoryMgrCtr.h"

namespace ssys {
namespace ma {
class VramHeap;
} // namespace ma
} // namespace ssys

namespace graphic {
// RTTI N7graphic9MemoryMgrE @ 0x008D3F20
// vtable 0x0090BCE4 (vptr 0x0090BCEC), offset_to_top 0, 6 entries
class MemoryMgr : public ::sead::GfxMemoryMgrCtr
{
public:
    MemoryMgr(); // ctor address unknown
    virtual void vf_0x00(); // 0x0075ECE8 slot 0x00 | virtual slot, introduced by graphic::MemoryMgr
    virtual void vf_0x04(); // 0x0075EC9C slot 0x04 | virtual slot, introduced by graphic::MemoryMgr
    virtual void vf_0x08(); // 0x00617AB0 slot 0x08 | virtual slot, introduced by graphic::MemoryMgr
    virtual void vf_0x0C(); // 0x00617A18 slot 0x0C | virtual slot, introduced by graphic::MemoryMgr
    virtual void vf_0x10(); // 0x00617B50 slot 0x10 | virtual slot, introduced by graphic::MemoryMgr
    virtual void vf_0x14(); // 0x00617B4C slot 0x14 | virtual slot, introduced by graphic::MemoryMgr

    // the instance (names are ours)
    static MemoryMgr* s_pInstance;

    u8 m_Unknown0x08[0xC];                       // 0x08 (+0x0C/+0x10: the heaps of VRAM A and B)
    ssys::ma::VramHeap* m_pCurrentHeaps[3];      // 0x14 the heaps the allocations of each kind come from (CurrentSetter)
};
} // namespace graphic
