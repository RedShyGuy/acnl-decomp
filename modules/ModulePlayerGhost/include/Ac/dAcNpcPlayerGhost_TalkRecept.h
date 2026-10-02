#pragma once

#include "decomp.h"
#include "Ac/dAcNpcPlayerGhost.h"
#include "Npc/dNpcTalkRecept.h"

// vtable +0x3204 in ModulePlayerGhost.cro, offset_to_top 0, 80 entries
// vtable +0x334C in ModulePlayerGhost.cro, offset_to_top -124, 14 entries
class AcNpcPlayerGhost::TalkRecept : public ::NpcTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModulePlayerGhost.cro +0x00106C slot 0x00
};
