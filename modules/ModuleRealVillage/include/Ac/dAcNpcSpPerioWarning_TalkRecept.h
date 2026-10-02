#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPerioWarning.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x151F8 in ModuleRealVillage.cro, offset_to_top 0, 91 entries
// vtable +0x1536C in ModuleRealVillage.cro, offset_to_top -124, 14 entries
class AcNpcSpPerioWarning::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleRealVillage.cro +0x008214 slot 0x00
};
