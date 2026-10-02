#pragma once

#include "decomp.h"
#include "Npc/dNpcModel.h"

// RTTI 18NpcTopsChangeModel @ 0x008CC8A4
// vtable 0x008F4A3C (vptr 0x008F4A44), offset_to_top 0, 20 entries
class NpcTopsChangeModel : public ::NpcModel
{
public:
    NpcTopsChangeModel(); // ctor address unknown
    virtual ~NpcTopsChangeModel(); // 0x002E600C slot 0x1C | slot vf_0x1C of HumanModel
    // 0x002E5FFC slot 0x20 | slot vf_0x20 of HumanModel (deleting dtor)
};
