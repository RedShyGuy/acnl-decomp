#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpJingle.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x12C4C in ModuleWinter.cro, offset_to_top 0, 91 entries
// vtable +0x12DC0 in ModuleWinter.cro, offset_to_top -124, 14 entries
class AcNpcSpJingle::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleWinter.cro +0x00BAF8 slot 0x00
};
