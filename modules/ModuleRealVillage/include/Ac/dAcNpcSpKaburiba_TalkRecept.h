#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpKaburiba.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x14B1C in ModuleRealVillage.cro, offset_to_top 0, 91 entries
// vtable +0x14C90 in ModuleRealVillage.cro, offset_to_top -124, 14 entries
class AcNpcSpKaburiba::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleRealVillage.cro +0x005B9C slot 0x00
};
