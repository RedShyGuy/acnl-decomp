#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpSecretaryTutorialWP.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x694E0 in ModuleIndoor.cro, offset_to_top 0, 91 entries
// vtable +0x69654 in ModuleIndoor.cro, offset_to_top -124, 14 entries
class AcNpcSpSecretaryTutorialWP::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleIndoor.cro +0x041B34 slot 0x00
};
