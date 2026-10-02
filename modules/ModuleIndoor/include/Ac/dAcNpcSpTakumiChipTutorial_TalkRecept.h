#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpTakumiChipTutorial.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x6932C in ModuleIndoor.cro, offset_to_top 0, 91 entries
// vtable +0x694A0 in ModuleIndoor.cro, offset_to_top -124, 14 entries
class AcNpcSpTakumiChipTutorial::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleIndoor.cro +0x040E80 slot 0x00
};
