#pragma once

#include "decomp.h"
#include "sead/seadHeap.h"

namespace sead {
// RTTI N4sead8UnitHeapE @ 0x008D224C
// vtable 0x00906E80 (vptr 0x00906E88), offset_to_top 0, 25 entries
// vtable 0x00906EEC (vptr 0x00906EF4), offset_to_top -24, 1 entries
class UnitHeap : public ::sead::Heap
{
public:
    UnitHeap(); // ctor candidate(s) 0x0012D004, 0x002F7020 (unverified)
    virtual ~UnitHeap(); // 0x00561D1C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00561CEC slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x08(); // 0x0074FC68 slot 0x08 | virtual slot, introduced by sead::Heap
    virtual void vf_0x0C(); // 0x0074FC08 slot 0x0C | virtual slot, introduced by sead::Heap
    virtual void vf_0x10(); // 0x00561AA0 slot 0x10 | virtual slot, introduced by sead::Heap
    virtual void vf_0x14(); // 0x00561A98 slot 0x14 | virtual slot, introduced by sead::Heap
    virtual void vf_0x18(); // 0x00561BD0 slot 0x18 | virtual slot, introduced by sead::Heap
    virtual void vf_0x1C(); // 0x00561A0C slot 0x1C | virtual slot, introduced by sead::Heap
    virtual void vf_0x20(); // 0x00561A00 slot 0x20 | virtual slot, introduced by sead::Heap
    virtual void vf_0x24(); // 0x005619F8 slot 0x24 | virtual slot, introduced by sead::Heap
    virtual void vf_0x2C(); // 0x00561B10 slot 0x2C | virtual slot, introduced by sead::Heap
    virtual void vf_0x30(); // 0x0074FC00 slot 0x30 | virtual slot, introduced by sead::Heap
    virtual void vf_0x34(); // 0x0074FBF0 slot 0x34 | virtual slot, introduced by sead::Heap
    virtual void vf_0x38(); // 0x0074FD60 slot 0x38 | virtual slot, introduced by sead::Heap
    virtual void vf_0x3C(); // 0x0074FBD8 slot 0x3C | virtual slot, introduced by sead::Heap
    virtual void vf_0x40(); // 0x0074FC54 slot 0x40 | virtual slot, introduced by sead::Heap
    virtual void vf_0x44(); // 0x0074FFCC slot 0x44 | virtual slot, introduced by sead::Heap
    virtual void vf_0x48(); // 0x0074FBD0 slot 0x48 | virtual slot, introduced by sead::Heap
    virtual void vf_0x4C(); // 0x0074FBE0 slot 0x4C | virtual slot, introduced by sead::Heap
    virtual void vf_0x50(); // 0x0074FBE8 slot 0x50 | virtual slot, introduced by sead::Heap
    virtual void vf_0x54(); // 0x0074FD1C slot 0x54 | virtual slot, introduced by sead::Heap
    virtual void vf_0x58(); // 0x0074FD68 slot 0x58 | virtual slot, introduced by sead::Heap
    virtual void vf_0x5C(); // 0x00561A08 slot 0x5C | virtual slot, introduced by sead::Heap
};
} // namespace sead
