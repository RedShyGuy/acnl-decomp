#pragma once

#include "decomp.h"
#include "Other/dEventDemoStationSeqRecepter.h"

// vtable +0xCBEC in ModuleStation.cro, offset_to_top 0, 87 entries
// vtable +0xCD50 in ModuleStation.cro, offset_to_top -124, 14 entries
class StationEntrySeqTalkRecept : public ::EventDemoStationSeqRecepter
{
public:
    StationEntrySeqTalkRecept(); // ctor address unknown
    virtual ~StationEntrySeqTalkRecept(); // ModuleStation.cro +0x007774 slot 0x00
};
