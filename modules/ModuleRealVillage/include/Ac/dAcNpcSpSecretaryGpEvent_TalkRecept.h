#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpSecretaryGpEvent.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x15FE4 in ModuleRealVillage.cro, offset_to_top 0, 91 entries
// vtable +0x16158 in ModuleRealVillage.cro, offset_to_top -124, 14 entries
class AcNpcSpSecretaryGpEvent::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleRealVillage.cro +0x010884 slot 0x00
};
