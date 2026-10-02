#pragma once

#include "decomp.h"
#include "Npc/dNpcSimpleModel.h"

// RTTI 22NpcBoardingSimpleModel @ 0x008CCE24
// vtable 0x008F6BE4 (vptr 0x008F6BEC), offset_to_top 0, 20 entries
class NpcBoardingSimpleModel : public ::NpcSimpleModel
{
public:
    NpcBoardingSimpleModel(); // ctor address unknown
    virtual ~NpcBoardingSimpleModel(); // 0x00331D94 slot 0x1C | slot vf_0x1C of HumanModel
    // 0x00331D84 slot 0x20 | slot vf_0x20 of HumanModel (deleting dtor)
    virtual void vf_0x24(); // 0x00331B84 slot 0x24 | virtual slot, introduced by HumanModel
};
