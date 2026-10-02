#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpSecretaryCeremony.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x589C in ModuleCeremony.cro, offset_to_top 0, 91 entries
// vtable +0x5A10 in ModuleCeremony.cro, offset_to_top -124, 14 entries
class AcNpcSpSecretaryCeremony::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleCeremony.cro +0x000F58 slot 0x00
};
