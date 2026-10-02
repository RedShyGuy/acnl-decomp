#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPyontarou.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x4890 in ModuleSummer.cro, offset_to_top 0, 91 entries
// vtable +0x4A04 in ModuleSummer.cro, offset_to_top -124, 14 entries
class AcNpcSpPyontarou::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleSummer.cro +0x001794 slot 0x00
};
