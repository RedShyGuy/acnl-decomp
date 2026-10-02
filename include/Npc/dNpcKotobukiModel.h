#pragma once

#include "decomp.h"
#include "Npc/dNpcTopsChangeModel.h"

// RTTI 16NpcKotobukiModel @ 0x008CC35C
// vtable 0x008F2AE0 (vptr 0x008F2AE8), offset_to_top 0, 20 entries
class NpcKotobukiModel : public ::NpcTopsChangeModel
{
public:
    NpcKotobukiModel(); // ctor address unknown
    virtual void vf_0x00(); // 0x002BB4CC slot 0x00 | virtual slot, introduced by HumanModel
    virtual ~NpcKotobukiModel(); // 0x002BB58C slot 0x1C | slot vf_0x1C of HumanModel
    // 0x002BB57C slot 0x20 | slot vf_0x20 of HumanModel (deleting dtor)
};
