#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpTrain.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0xF8A8 in ModuleTrain.cro, offset_to_top 0, 91 entries
// vtable +0xFA1C in ModuleTrain.cro, offset_to_top -124, 14 entries
class AcNpcSpTrain::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleTrain.cro +0x0027F0 slot 0x00
};
