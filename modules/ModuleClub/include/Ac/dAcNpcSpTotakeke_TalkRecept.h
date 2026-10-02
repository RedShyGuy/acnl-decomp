#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpTotakeke.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x1477C in ModuleClub.cro, offset_to_top 0, 91 entries
// vtable +0x148F0 in ModuleClub.cro, offset_to_top -124, 14 entries
class AcNpcSpTotakeke::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleClub.cro +0x006D54 slot 0x00
};
