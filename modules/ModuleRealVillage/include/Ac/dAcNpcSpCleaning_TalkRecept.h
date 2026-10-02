#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpCleaning.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x147B4 in ModuleRealVillage.cro, offset_to_top 0, 91 entries
// vtable +0x14928 in ModuleRealVillage.cro, offset_to_top -124, 14 entries
class AcNpcSpCleaning::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleRealVillage.cro +0x0036C0 slot 0x00
};
