#pragma once

#include "decomp.h"
#include "Ac/dAcNpcCamp.h"
#include "Npc/dNpcTalkRecept.h"

// vtable +0x69C08 in ModuleIndoor.cro, offset_to_top 0, 81 entries
// vtable +0x69D54 in ModuleIndoor.cro, offset_to_top -124, 14 entries
class AcNpcCamp::CampTalkRcpt : public ::NpcTalkRecept
{
public:
    CampTalkRcpt(); // ctor address unknown
    virtual ~CampTalkRcpt(); // ModuleIndoor.cro +0x0588D8 slot 0x00
};
