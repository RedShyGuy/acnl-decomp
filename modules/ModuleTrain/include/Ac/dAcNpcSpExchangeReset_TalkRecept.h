#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpExchangeReset.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0xFB00 in ModuleTrain.cro, offset_to_top 0, 92 entries
// vtable +0xFC78 in ModuleTrain.cro, offset_to_top -124, 14 entries
class AcNpcSpExchangeReset::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleTrain.cro +0x009534 slot 0x00
};
