#pragma once

#include "decomp.h"
#include "Npc/dNpcDemoDollTalkRecept.h"

// vtable +0xA35C in ModuleEventDemo.cro, offset_to_top 0, 87 entries
// vtable +0xA4C0 in ModuleEventDemo.cro, offset_to_top -124, 14 entries
class FishAwardSeqTalkRecept : public ::NpcDemoDollTalkRecept
{
public:
    FishAwardSeqTalkRecept(); // ctor address unknown
    virtual ~FishAwardSeqTalkRecept(); // ModuleEventDemo.cro +0x003DD0 slot 0x00
};
