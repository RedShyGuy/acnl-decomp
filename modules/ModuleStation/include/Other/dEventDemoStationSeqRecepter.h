#pragma once

#include "decomp.h"
#include "Npc/dNpcDemoDollTalkRecept.h"

// vtable +0xCF34 in ModuleStation.cro, offset_to_top 0, 87 entries
// vtable +0xD098 in ModuleStation.cro, offset_to_top -124, 14 entries
class EventDemoStationSeqRecepter : public ::NpcDemoDollTalkRecept
{
public:
    EventDemoStationSeqRecepter(); // ctor address unknown
    virtual ~EventDemoStationSeqRecepter(); // ModuleStation.cro +0x008C14 slot 0x00
};
