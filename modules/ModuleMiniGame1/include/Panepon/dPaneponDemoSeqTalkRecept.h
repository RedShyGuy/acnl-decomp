#pragma once

#include "decomp.h"
#include "Npc/dNpcDemoDollTalkRecept.h"

// vtable +0x3103C in ModuleMiniGame1.cro, offset_to_top 0, 88 entries
// vtable +0x311A4 in ModuleMiniGame1.cro, offset_to_top -124, 14 entries
class PaneponDemoSeqTalkRecept : public ::NpcDemoDollTalkRecept
{
public:
    PaneponDemoSeqTalkRecept(); // ctor address unknown
    virtual ~PaneponDemoSeqTalkRecept(); // ModuleMiniGame1.cro +0x02115C slot 0x00
};
