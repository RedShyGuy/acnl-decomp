#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpRakosuke.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x98958 in ModuleOutdoor.cro, offset_to_top 0, 91 entries
// vtable +0x98ACC in ModuleOutdoor.cro, offset_to_top -124, 14 entries
class AcNpcSpRakosuke::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleOutdoor.cro +0x031B90 slot 0x00
};
