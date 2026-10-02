#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpSecretaryTutorial.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x69178 in ModuleIndoor.cro, offset_to_top 0, 91 entries
// vtable +0x692EC in ModuleIndoor.cro, offset_to_top -124, 14 entries
class AcNpcSpSecretaryTutorial::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleIndoor.cro +0x03FB8C slot 0x00
};
