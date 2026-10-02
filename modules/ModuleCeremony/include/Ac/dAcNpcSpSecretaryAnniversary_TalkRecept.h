#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpSecretaryAnniversary.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x5A50 in ModuleCeremony.cro, offset_to_top 0, 91 entries
// vtable +0x5BC4 in ModuleCeremony.cro, offset_to_top -124, 14 entries
class AcNpcSpSecretaryAnniversary::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleCeremony.cro +0x003464 slot 0x00
};
