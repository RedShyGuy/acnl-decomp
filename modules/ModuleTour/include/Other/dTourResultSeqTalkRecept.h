#pragma once

#include "decomp.h"
#include "Npc/dNpcDemoDollTalkRecept.h"

// vtable +0x116EC in ModuleTour.cro, offset_to_top 0, 87 entries
// vtable +0x11850 in ModuleTour.cro, offset_to_top -124, 14 entries
class TourResultSeqTalkRecept : public ::NpcDemoDollTalkRecept
{
public:
    TourResultSeqTalkRecept(); // ctor address unknown
    virtual ~TourResultSeqTalkRecept(); // ModuleTour.cro +0x00AA4C slot 0x00
};
