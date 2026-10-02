#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpTunekichiEvent.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x15AA8 in ModuleRealVillage.cro, offset_to_top 0, 99 entries
// vtable +0x15C3C in ModuleRealVillage.cro, offset_to_top -124, 14 entries
class AcNpcSpTunekichiEvent::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleRealVillage.cro +0x00D648 slot 0x00
};
