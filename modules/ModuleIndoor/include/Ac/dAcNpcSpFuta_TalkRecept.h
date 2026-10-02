#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpFuta.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x67EAC in ModuleIndoor.cro, offset_to_top 0, 91 entries
// vtable +0x68020 in ModuleIndoor.cro, offset_to_top -124, 14 entries
class AcNpcSpFuta::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleIndoor.cro +0x008830 slot 0x00
};
