#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpResetsanIn.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x69B8 in ModuleReMoCnt.cro, offset_to_top 0, 91 entries
// vtable +0x6B2C in ModuleReMoCnt.cro, offset_to_top -124, 14 entries
class AcNpcSpResetsanIn::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleReMoCnt.cro +0x00345C slot 0x00
};
