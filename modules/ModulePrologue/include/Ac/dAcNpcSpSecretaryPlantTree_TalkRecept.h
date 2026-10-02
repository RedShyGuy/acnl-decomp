#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpSecretaryPlantTree.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x8F0C in ModulePrologue.cro, offset_to_top 0, 91 entries
// vtable +0x9080 in ModulePrologue.cro, offset_to_top -124, 14 entries
class AcNpcSpSecretaryPlantTree::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModulePrologue.cro +0x006420 slot 0x00
};
