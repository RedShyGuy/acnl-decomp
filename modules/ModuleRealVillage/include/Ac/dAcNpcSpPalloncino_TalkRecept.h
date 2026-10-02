#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPalloncino.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x14CD0 in ModuleRealVillage.cro, offset_to_top 0, 91 entries
// vtable +0x14E44 in ModuleRealVillage.cro, offset_to_top -124, 14 entries
class AcNpcSpPalloncino::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleRealVillage.cro +0x006FEC slot 0x00
};
