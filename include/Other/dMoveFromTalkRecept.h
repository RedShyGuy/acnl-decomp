#pragma once

#include "decomp.h"
#include "Other/dMoveFromTalkReceptBase.h"

// RTTI 18MoveFromTalkRecept @ 0x008CC868
// vtable 0x008F47C0 (vptr 0x008F47C8), offset_to_top 0, 66 entries
// vtable 0x008F48D0 (vptr 0x008F48D8), offset_to_top -124, 14 entries
class MoveFromTalkRecept : public ::MoveFromTalkReceptBase
{
public:
    MoveFromTalkRecept(); // ctor address unknown
    virtual ~MoveFromTalkRecept(); // 0x002E5E24 slot 0x00 | slot vf_0x00 of script::ITalkRecept
    // 0x002E5DD8 slot 0x04 | slot vf_0x04 of script::ITalkRecept (deleting dtor)
    virtual void vf_0x94(); // 0x002CF988 slot 0x94 | virtual slot, introduced by script::ITalkRecept
    virtual void vf_0x100(); // 0x002E52E0 slot 0x100 | virtual slot, introduced by MoveFromTalkRecept
    virtual void vf_0x104(); // 0x002E4EB0 slot 0x104 | virtual slot, introduced by MoveFromTalkRecept
};
