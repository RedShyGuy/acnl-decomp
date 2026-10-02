#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpMaster.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0xDB78 in ModuleCafe.cro, offset_to_top 0, 91 entries
// vtable +0xDCEC in ModuleCafe.cro, offset_to_top -124, 14 entries
class AcNpcSpMaster::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleCafe.cro +0x0044A8 slot 0x00
};
