#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpSecretaryPWorks.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x15C7C in ModuleRealVillage.cro, offset_to_top 0, 91 entries
// vtable +0x15DF0 in ModuleRealVillage.cro, offset_to_top -124, 14 entries
class AcNpcSpSecretaryPWorks::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleRealVillage.cro +0x00EFFC slot 0x00
};
