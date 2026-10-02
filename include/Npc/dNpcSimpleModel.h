#pragma once

#include "decomp.h"
#include "Npc/dNpcModel.h"

// RTTI 14NpcSimpleModel @ 0x008CBD2C
// vtable 0x008F0548 (vptr 0x008F0550), offset_to_top 0, 20 entries
class NpcSimpleModel : public ::NpcModel
{
public:
    NpcSimpleModel(); // ctor address unknown
    virtual ~NpcSimpleModel(); // 0x00270B0C slot 0x1C | slot vf_0x1C of HumanModel
    // 0x00270AFC slot 0x20 | slot vf_0x20 of HumanModel (deleting dtor)
};
