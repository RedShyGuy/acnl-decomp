#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpSecretaryEvent.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x158C8 in ModuleRealVillage.cro, offset_to_top 0, 102 entries
// vtable +0x15A68 in ModuleRealVillage.cro, offset_to_top -124, 14 entries
class AcNpcSpSecretaryEvent::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleRealVillage.cro +0x00C360 slot 0x00
};
