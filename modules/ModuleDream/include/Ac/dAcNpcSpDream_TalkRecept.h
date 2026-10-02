#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpDream.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x138AC in ModuleDream.cro, offset_to_top 0, 91 entries
// vtable +0x13A20 in ModuleDream.cro, offset_to_top -124, 14 entries
class AcNpcSpDream::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleDream.cro +0x004668 slot 0x00
};
