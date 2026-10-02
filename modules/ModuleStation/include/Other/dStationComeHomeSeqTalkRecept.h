#pragma once

#include "decomp.h"
#include "Other/dEventDemoStationSeqRecepter.h"

// vtable +0xD0D8 in ModuleStation.cro, offset_to_top 0, 87 entries
// vtable +0xD23C in ModuleStation.cro, offset_to_top -124, 14 entries
class StationComeHomeSeqTalkRecept : public ::EventDemoStationSeqRecepter
{
public:
    StationComeHomeSeqTalkRecept(); // ctor address unknown
    virtual ~StationComeHomeSeqTalkRecept(); // ModuleStation.cro +0x008E58 slot 0x00
};
