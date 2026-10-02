#pragma once

#include "decomp.h"
#include "Npc/dNpcDemoDollTalkRecept.h"

// vtable +0xA6A4 in ModuleEventDemo.cro, offset_to_top 0, 87 entries
// vtable +0xA808 in ModuleEventDemo.cro, offset_to_top -124, 14 entries
class SeaDepartureSeqTalkRecept : public ::NpcDemoDollTalkRecept
{
public:
    SeaDepartureSeqTalkRecept(); // ctor address unknown
    virtual ~SeaDepartureSeqTalkRecept(); // ModuleEventDemo.cro +0x006218 slot 0x00
};
