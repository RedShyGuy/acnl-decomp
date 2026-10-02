#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpDJKK.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x14414 in ModuleClub.cro, offset_to_top 0, 91 entries
// vtable +0x14588 in ModuleClub.cro, offset_to_top -124, 14 entries
class AcNpcSpDJKK::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleClub.cro +0x0014A8 slot 0x00
};
