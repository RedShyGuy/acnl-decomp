#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPerioNormal.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x14E84 in ModuleRealVillage.cro, offset_to_top 0, 91 entries
// vtable +0x14FF8 in ModuleRealVillage.cro, offset_to_top -124, 14 entries
class AcNpcSpPerioNormal::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleRealVillage.cro +0x0075A8 slot 0x00
};
