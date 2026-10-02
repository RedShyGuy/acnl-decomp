#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpSecretaryIn.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x68BB8 in ModuleIndoor.cro, offset_to_top 0, 91 entries
// vtable +0x68D2C in ModuleIndoor.cro, offset_to_top -124, 14 entries
class AcNpcSpSecretaryIn::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleIndoor.cro +0x02B460 slot 0x00
};
