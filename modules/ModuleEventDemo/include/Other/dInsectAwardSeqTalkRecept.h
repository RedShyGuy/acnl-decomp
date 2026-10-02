#pragma once

#include "decomp.h"
#include "Npc/dNpcDemoDollTalkRecept.h"

// vtable +0xA500 in ModuleEventDemo.cro, offset_to_top 0, 87 entries
// vtable +0xA664 in ModuleEventDemo.cro, offset_to_top -124, 14 entries
class InsectAwardSeqTalkRecept : public ::NpcDemoDollTalkRecept
{
public:
    InsectAwardSeqTalkRecept(); // ctor address unknown
    virtual ~InsectAwardSeqTalkRecept(); // ModuleEventDemo.cro +0x0049E0 slot 0x00
};
