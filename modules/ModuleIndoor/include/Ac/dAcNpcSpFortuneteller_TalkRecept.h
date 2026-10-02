#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpFortuneteller.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x68D6C in ModuleIndoor.cro, offset_to_top 0, 91 entries
// vtable +0x68EE0 in ModuleIndoor.cro, offset_to_top -124, 14 entries
class AcNpcSpFortuneteller::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleIndoor.cro +0x03AE9C slot 0x00
};
