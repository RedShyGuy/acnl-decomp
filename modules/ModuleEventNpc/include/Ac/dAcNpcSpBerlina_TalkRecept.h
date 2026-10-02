#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpBerlina.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x3390 in ModuleEventNpc.cro, offset_to_top 0, 91 entries
// vtable +0x3504 in ModuleEventNpc.cro, offset_to_top -124, 14 entries
class AcNpcSpBerlina::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleEventNpc.cro +0x001548 slot 0x00
};
