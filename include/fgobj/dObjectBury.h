#pragma once

#include "decomp.h"
#include "fgobj/dObjectExe.h"

namespace fgobj {
// RTTI N5fgobj10ObjectBuryE @ 0x008D291C
// vtable 0x00908A64 (vptr 0x00908A6C), offset_to_top 0, 10 entries
class ObjectBury : public ::fgobj::ObjectExe
{
public:
    ObjectBury(); // ctor candidate(s) 0x0059ADC0 (unverified)
    virtual ~ObjectBury(); // 0x00591DEC slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00591DBC slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x20(); // 0x00591BCC slot 0x20 | virtual slot, introduced by fgobj::ObjectBury
    virtual void vf_0x24(); // 0x005A3638 slot 0x24 | virtual slot, introduced by fgobj::ObjectBury
};
} // namespace fgobj
