#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpSeiichi.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x13A70 in ModuleDream.cro, offset_to_top 0, 91 entries
// vtable +0x13BE4 in ModuleDream.cro, offset_to_top -124, 14 entries
class AcNpcSpSeiichi::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleDream.cro +0x006DF8 slot 0x00
};
