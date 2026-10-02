#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpSisyo.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x145C8 in ModuleClub.cro, offset_to_top 0, 91 entries
// vtable +0x1473C in ModuleClub.cro, offset_to_top -124, 14 entries
class AcNpcSpSisyo::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleClub.cro +0x003004 slot 0x00
};
