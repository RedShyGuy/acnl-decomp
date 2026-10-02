#pragma once

#include "decomp.h"
#include "Other/dInstSelect.h"

// vtable +0xD420 in ModuleStation.cro, offset_to_top 0, 26 entries
class TripList : public ::InstSelect<10>
{
public:
    TripList(); // ctor address unknown
    virtual ~TripList(); // ModuleStation.cro +0x00A3A4 slot 0x00
};
