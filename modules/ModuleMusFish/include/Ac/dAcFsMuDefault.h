#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"

// vtable +0x2D2A8 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2D36C in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2D3C8 in ModuleMusFish.cro, offset_to_top -552, 11 entries
class AcFsMuDefault : public ::AcFishMuseumBase
{
public:
    AcFsMuDefault(); // ctor address unknown
    virtual ~AcFsMuDefault(); // ModuleMusFish.cro +0x004DD8 slot 0x00
};
