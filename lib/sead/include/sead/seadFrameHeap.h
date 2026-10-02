#pragma once

#include "decomp.h"
#include "sead/seadHeap.h"

namespace sead {
// RTTI N4sead9FrameHeapE @ 0x008D2368
// vtable 0x00907048 (vptr 0x00907050), offset_to_top 0, 25 entries
// vtable 0x009070B4 (vptr 0x009070BC), offset_to_top -24, 1 entries
class FrameHeap : public ::sead::Heap
{
public:
    FrameHeap(); // ctor address unknown
    virtual ~FrameHeap(); // 0x005625D0 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x005625A0 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x08(); // 0x00750398 slot 0x08 | virtual slot, introduced by sead::Heap
    virtual void vf_0x0C(); // 0x007502CC slot 0x0C | virtual slot, introduced by sead::Heap
    virtual void vf_0x10(); // 0x005620E0 slot 0x10 | virtual slot, introduced by sead::Heap
    virtual void vf_0x14(); // 0x00561F88 slot 0x14 | virtual slot, introduced by sead::Heap
    virtual void vf_0x18(); // 0x005621C4 slot 0x18 | virtual slot, introduced by sead::Heap
    virtual void vf_0x1C(); // 0x00561F84 slot 0x1C | virtual slot, introduced by sead::Heap
    virtual void vf_0x20(); // 0x00561F78 slot 0x20 | virtual slot, introduced by sead::Heap
    virtual void vf_0x24(); // 0x00561F70 slot 0x24 | virtual slot, introduced by sead::Heap
    virtual void vf_0x2C(); // 0x00562134 slot 0x2C | virtual slot, introduced by sead::Heap
    virtual void vf_0x30(); // 0x007502C4 slot 0x30 | virtual slot, introduced by sead::Heap
    virtual void vf_0x34(); // 0x007502B4 slot 0x34 | virtual slot, introduced by sead::Heap
    virtual void vf_0x38(); // 0x00750490 slot 0x38 | virtual slot, introduced by sead::Heap
    virtual void vf_0x3C(); // 0x0075025C slot 0x3C | virtual slot, introduced by sead::Heap
    virtual void vf_0x40(); // 0x00750318 slot 0x40 | virtual slot, introduced by sead::Heap
    virtual void vf_0x44(); // 0x00750858 slot 0x44 | virtual slot, introduced by sead::Heap
    virtual void vf_0x48(); // 0x00750254 slot 0x48 | virtual slot, introduced by sead::Heap
    virtual void vf_0x4C(); // 0x007502A4 slot 0x4C | virtual slot, introduced by sead::Heap
    virtual void vf_0x50(); // 0x007502AC slot 0x50 | virtual slot, introduced by sead::Heap
    virtual void vf_0x54(); // 0x0075044C slot 0x54 | virtual slot, introduced by sead::Heap
    virtual void vf_0x58(); // 0x00750498 slot 0x58 | virtual slot, introduced by sead::Heap
    virtual void vf_0x5C(); // 0x00561F80 slot 0x5C | virtual slot, introduced by sead::Heap
};
} // namespace sead
