#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPumpking.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x12FC0 in ModuleWinter.cro, offset_to_top 0, 91 entries
// vtable +0x13134 in ModuleWinter.cro, offset_to_top -124, 14 entries
class AcNpcSpPumpking::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleWinter.cro +0x00EFB0 slot 0x00
};
