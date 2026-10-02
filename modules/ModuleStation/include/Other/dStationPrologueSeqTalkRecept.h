#pragma once

#include "decomp.h"
#include "Other/dEventDemoStationSeqRecepter.h"

// vtable +0xD27C in ModuleStation.cro, offset_to_top 0, 87 entries
// vtable +0xD3E0 in ModuleStation.cro, offset_to_top -124, 14 entries
class StationPrologueSeqTalkRecept : public ::EventDemoStationSeqRecepter
{
public:
    StationPrologueSeqTalkRecept(); // ctor address unknown
    virtual ~StationPrologueSeqTalkRecept(); // ModuleStation.cro +0x0094D8 slot 0x00
};
