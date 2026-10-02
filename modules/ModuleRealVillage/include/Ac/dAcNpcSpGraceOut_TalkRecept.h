#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpGraceOut.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x14968 in ModuleRealVillage.cro, offset_to_top 0, 91 entries
// vtable +0x14ADC in ModuleRealVillage.cro, offset_to_top -124, 14 entries
class AcNpcSpGraceOut::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleRealVillage.cro +0x005434 slot 0x00
};
