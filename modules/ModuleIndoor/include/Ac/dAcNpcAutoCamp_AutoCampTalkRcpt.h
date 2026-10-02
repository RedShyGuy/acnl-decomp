#pragma once

#include "decomp.h"
#include "Ac/dAcNpcAutoCamp.h"
#include "Npc/dNpcTalkRecept.h"

// vtable +0x68078 in ModuleIndoor.cro, offset_to_top 0, 81 entries
// vtable +0x681C4 in ModuleIndoor.cro, offset_to_top -124, 14 entries
class AcNpcAutoCamp::AutoCampTalkRcpt : public ::NpcTalkRecept
{
public:
    AutoCampTalkRcpt(); // ctor address unknown
    virtual ~AutoCampTalkRcpt(); // ModuleIndoor.cro +0x010300 slot 0x00
};
