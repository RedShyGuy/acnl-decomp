#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpCafeTalk.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0xDD38 in ModuleCafe.cro, offset_to_top 0, 92 entries
// vtable +0xDEB0 in ModuleCafe.cro, offset_to_top -124, 14 entries
class AcNpcSpCafeTalk::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleCafe.cro +0x005978 slot 0x00
};
