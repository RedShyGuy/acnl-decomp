#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPerioTutorial.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x15714 in ModuleRealVillage.cro, offset_to_top 0, 91 entries
// vtable +0x15888 in ModuleRealVillage.cro, offset_to_top -124, 14 entries
class AcNpcSpPerioTutorial::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleRealVillage.cro +0x00B5D4 slot 0x00
};
