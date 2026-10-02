#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPerioSpecial.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x15044 in ModuleRealVillage.cro, offset_to_top 0, 91 entries
// vtable +0x151B8 in ModuleRealVillage.cro, offset_to_top -124, 14 entries
class AcNpcSpPerioSpecial::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleRealVillage.cro +0x007CEC slot 0x00
};
