#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpYutarou.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x68588 in ModuleIndoor.cro, offset_to_top 0, 91 entries
// vtable +0x686FC in ModuleIndoor.cro, offset_to_top -124, 14 entries
class AcNpcSpYutarou::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleIndoor.cro +0x016C4C slot 0x00
};
