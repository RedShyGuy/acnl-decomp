#pragma once

#include "decomp.h"
#include "Ac/dAcNpcAutoCampOut.h"
#include "Npc/dNpcTalkRecept.h"

// vtable +0xE1E8 in ModuleAutoCamp.cro, offset_to_top 0, 81 entries
// vtable +0xE334 in ModuleAutoCamp.cro, offset_to_top -124, 14 entries
class AcNpcAutoCampOut::AutoCampOutTalkRcpt : public ::NpcTalkRecept
{
public:
    AutoCampOutTalkRcpt(); // ctor address unknown
    virtual ~AutoCampOutTalkRcpt(); // ModuleAutoCamp.cro +0x008838 slot 0x00
};
