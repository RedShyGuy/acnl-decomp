#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPlSelect.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x6730 in ModulePlSelect.cro, offset_to_top 0, 91 entries
// vtable +0x68A4 in ModulePlSelect.cro, offset_to_top -124, 14 entries
class AcNpcSpPlSelect::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModulePlSelect.cro +0x0035DC slot 0x00
};
