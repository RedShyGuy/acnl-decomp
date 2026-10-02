#pragma once

#include "decomp.h"
#include "Npc/dNpcSimpleModel.h"

// RTTI 24NpcResetChairSimpleModel @ 0x008CD018
// vtable 0x008F78E4 (vptr 0x008F78EC), offset_to_top 0, 20 entries
class NpcResetChairSimpleModel : public ::NpcSimpleModel
{
public:
    NpcResetChairSimpleModel(); // ctor address unknown
    virtual ~NpcResetChairSimpleModel(); // 0x0033E810 slot 0x1C | slot vf_0x1C of HumanModel
    // 0x0033E800 slot 0x20 | slot vf_0x20 of HumanModel (deleting dtor)
    virtual void vf_0x24(); // 0x0033E758 slot 0x24 | virtual slot, introduced by HumanModel
};
