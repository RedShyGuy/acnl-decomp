#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpMysteryCat.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x4A44 in ModuleSummer.cro, offset_to_top 0, 91 entries
// vtable +0x4BB8 in ModuleSummer.cro, offset_to_top -124, 14 entries
class AcNpcSpMysteryCat::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleSummer.cro +0x00282C slot 0x00
};
