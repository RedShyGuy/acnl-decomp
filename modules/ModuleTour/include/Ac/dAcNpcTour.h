#pragma once

#include "decomp.h"
#include "Ac/dAcNpc.h"

// vtable +0x11E60 in ModuleTour.cro, offset_to_top 0, 89 entries
class AcNpcTour : public ::AcNpc
{
public:
    class TourTalkRcpt;
    AcNpcTour(); // ctor address unknown
    virtual ~AcNpcTour(); // ModuleTour.cro +0x00CED8 slot 0x00
};
