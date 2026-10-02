#pragma once

#include "decomp.h"
#include "Npc/dNpcSimpleModel.h"

// RTTI 15NpcDcoAnimModel @ 0x008CC0EC
// vtable 0x008F1BD0 (vptr 0x008F1BD8), offset_to_top 0, 20 entries
class NpcDcoAnimModel : public ::NpcSimpleModel
{
public:
    NpcDcoAnimModel(); // ctor address unknown
    virtual void vf_0x00(); // 0x0029CF4C slot 0x00 | virtual slot, introduced by HumanModel
    virtual void vf_0x0C(); // 0x0029CF90 slot 0x0C | virtual slot, introduced by HumanModel
    virtual ~NpcDcoAnimModel(); // 0x0029D0D4 slot 0x1C | slot vf_0x1C of HumanModel
    // 0x0029D0C4 slot 0x20 | slot vf_0x20 of HumanModel (deleting dtor)
};
