#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpUomasa.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x55DC in ModuleFsInsAward.cro, offset_to_top 0, 91 entries
// vtable +0x5750 in ModuleFsInsAward.cro, offset_to_top -124, 14 entries
class AcNpcSpUomasa::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleFsInsAward.cro +0x001FB0 slot 0x00
};
