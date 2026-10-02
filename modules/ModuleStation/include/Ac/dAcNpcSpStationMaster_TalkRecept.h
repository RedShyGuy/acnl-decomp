#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpStationMaster.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0xD490 in ModuleStation.cro, offset_to_top 0, 91 entries
// vtable +0xD604 in ModuleStation.cro, offset_to_top -124, 14 entries
class AcNpcSpStationMaster::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleStation.cro +0x005680 slot 0x00
};
