#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpSecretaryOut.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x8BA4 in ModulePrologue.cro, offset_to_top 0, 91 entries
// vtable +0x8D18 in ModulePrologue.cro, offset_to_top -124, 14 entries
class AcNpcSpSecretaryOut::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModulePrologue.cro +0x0025E0 slot 0x00
};
