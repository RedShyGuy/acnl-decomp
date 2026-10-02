#pragma once

#include "decomp.h"
#include "fgobj/dObjectExe.h"

namespace fgobj {
// RTTI N5fgobj11ObjectPlantE @ 0x008D297C
// vtable 0x00908B64 (vptr 0x00908B6C), offset_to_top 0, 10 entries
class ObjectPlant : public ::fgobj::ObjectExe
{
public:
    ObjectPlant(); // ctor candidate(s) 0x00592CC8 (unverified)
    virtual ~ObjectPlant(); // 0x00592EB8 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00592E88 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x20(); // 0x00592DB8 slot 0x20 | virtual slot, introduced by fgobj::ObjectPlant
    virtual void vf_0x24(); // 0x005A3638 slot 0x24 | virtual slot, introduced by fgobj::ObjectBury
};
} // namespace fgobj
