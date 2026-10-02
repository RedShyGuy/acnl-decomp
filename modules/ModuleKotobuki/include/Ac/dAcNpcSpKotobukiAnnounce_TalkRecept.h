#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpKotobukiAnnounce.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x46BC in ModuleKotobuki.cro, offset_to_top 0, 91 entries
// vtable +0x4830 in ModuleKotobuki.cro, offset_to_top -124, 14 entries
class AcNpcSpKotobukiAnnounce::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleKotobuki.cro +0x0025C4 slot 0x00
};
