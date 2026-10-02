#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSpKappei.h"
#include "Npc/dNpcSpTalkRecept.h"

// vtable +0x33C0 in ModuleKappei.cro, offset_to_top 0, 92 entries
// vtable +0x3538 in ModuleKappei.cro, offset_to_top -124, 14 entries
class AcNpcSpKappei::TalkRecept : public ::NpcSpTalkRecept
{
public:
    TalkRecept(); // ctor address unknown
    virtual ~TalkRecept(); // ModuleKappei.cro +0x001358 slot 0x00
};
