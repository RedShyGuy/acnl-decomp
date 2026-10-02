#pragma once

#include "decomp.h"
#include "Ac/dAcNpcTour.h"
#include "Npc/dNpcTalkRecept.h"

// vtable +0x13A08 in ModuleTour.cro, offset_to_top 0, 81 entries
// vtable +0x13B54 in ModuleTour.cro, offset_to_top -124, 14 entries
class AcNpcTour::TourTalkRcpt : public ::NpcTalkRecept
{
public:
    TourTalkRcpt(); // ctor address unknown
    virtual ~TourTalkRcpt(); // ModuleTour.cro +0x00C624 slot 0x00
};
