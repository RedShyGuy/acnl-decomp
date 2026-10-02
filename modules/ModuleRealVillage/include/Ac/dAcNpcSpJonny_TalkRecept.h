#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpJonny.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x14548 in ModuleRealVillage.cro, offset_to_top 0, 91 entries
// vtable +0x146BC in ModuleRealVillage.cro, offset_to_top -124, 14 entries
class AcNpcSpJonny::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleRealVillage.cro +0x002918 slot 0x00
};
