#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpCleaningVisit.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x15560 in ModuleRealVillage.cro, offset_to_top 0, 91 entries
// vtable +0x156D4 in ModuleRealVillage.cro, offset_to_top -124, 14 entries
class AcNpcSpCleaningVisit::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleRealVillage.cro +0x00ABEC slot 0x00
};
