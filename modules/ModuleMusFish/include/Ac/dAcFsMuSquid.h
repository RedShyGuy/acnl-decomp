#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumSea.h"
#include "Object/dObjectState.h"

// vtable +0x2CD3C in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2CE00 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2CE34 in ModuleMusFish.cro, offset_to_top -632, 2 entries
// vtable +0x2CE6C in ModuleMusFish.cro, offset_to_top -808, 11 entries
class AcFsMuSquid : public ::AcFishMuseumSea, public ::ObjectState<AcFsMuSquid>
{
public:
    AcFsMuSquid(); // ctor address unknown
    virtual ~AcFsMuSquid(); // ModuleMusFish.cro +0x011150 slot 0x00
};
