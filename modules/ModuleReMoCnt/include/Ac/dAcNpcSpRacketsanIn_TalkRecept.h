#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpRacketsanIn.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x6B6C in ModuleReMoCnt.cro, offset_to_top 0, 91 entries
// vtable +0x6CE0 in ModuleReMoCnt.cro, offset_to_top -124, 14 entries
class AcNpcSpRacketsanIn::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleReMoCnt.cro +0x004664 slot 0x00
};
