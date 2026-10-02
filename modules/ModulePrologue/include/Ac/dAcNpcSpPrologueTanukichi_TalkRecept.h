#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpPrologueTanukichi.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x8D58 in ModulePrologue.cro, offset_to_top 0, 91 entries
// vtable +0x8ECC in ModulePrologue.cro, offset_to_top -124, 14 entries
class AcNpcSpPrologueTanukichi::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModulePrologue.cro +0x004EEC slot 0x00
};
