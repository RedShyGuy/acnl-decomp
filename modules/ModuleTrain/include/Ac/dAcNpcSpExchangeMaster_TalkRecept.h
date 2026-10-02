#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpExchangeMaster.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0xFCC4 in ModuleTrain.cro, offset_to_top 0, 92 entries
// vtable +0xFE3C in ModuleTrain.cro, offset_to_top -124, 14 entries
class AcNpcSpExchangeMaster::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleTrain.cro +0x00A368 slot 0x00
};
