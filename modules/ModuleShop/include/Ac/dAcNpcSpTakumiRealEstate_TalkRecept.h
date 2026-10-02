#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpTakumiRealEstate.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x29B48 in ModuleShop.cro, offset_to_top 0, 91 entries
// vtable +0x29CBC in ModuleShop.cro, offset_to_top -124, 14 entries
class AcNpcSpTakumiRealEstate::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleShop.cro +0x02234C slot 0x00
};
