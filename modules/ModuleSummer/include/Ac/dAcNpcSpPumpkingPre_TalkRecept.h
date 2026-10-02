#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPumpkingPre.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x4BF8 in ModuleSummer.cro, offset_to_top 0, 91 entries
// vtable +0x4D6C in ModuleSummer.cro, offset_to_top -124, 14 entries
class AcNpcSpPumpkingPre::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleSummer.cro +0x003454 slot 0x00
};
