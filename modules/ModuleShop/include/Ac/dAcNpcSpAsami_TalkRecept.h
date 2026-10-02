#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpAsami.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x281D8 in ModuleShop.cro, offset_to_top 0, 91 entries
// vtable +0x2834C in ModuleShop.cro, offset_to_top -124, 14 entries
class AcNpcSpAsami::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleShop.cro +0x002E84 slot 0x00
};
