#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpFrankrin.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x12E0C in ModuleWinter.cro, offset_to_top 0, 91 entries
// vtable +0x12F80 in ModuleWinter.cro, offset_to_top -124, 14 entries
class AcNpcSpFrankrin::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleWinter.cro +0x00DD88 slot 0x00
};
