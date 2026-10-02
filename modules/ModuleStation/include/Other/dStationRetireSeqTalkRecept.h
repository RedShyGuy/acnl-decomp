#pragma once

#include "decomp.h"
#include "Other/dEventDemoStationSeqRecepter.h"

// vtable +0xCD90 in ModuleStation.cro, offset_to_top 0, 87 entries
// vtable +0xCEF4 in ModuleStation.cro, offset_to_top -124, 14 entries
class StationRetireSeqTalkRecept : public ::EventDemoStationSeqRecepter
{
public:
    StationRetireSeqTalkRecept(); // ctor address unknown
    virtual ~StationRetireSeqTalkRecept(); // ModuleStation.cro +0x007C80 slot 0x00
};
