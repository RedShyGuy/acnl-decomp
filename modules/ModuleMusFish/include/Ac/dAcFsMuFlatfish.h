#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumSea.h"
#include "Object/dObjectState.h"

// vtable +0x2DE84 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2DF48 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2DF7C in ModuleMusFish.cro, offset_to_top -632, 2 entries
// vtable +0x2DFB4 in ModuleMusFish.cro, offset_to_top -892, 11 entries
class AcFsMuFlatfish : public ::AcFishMuseumSea, public ::ObjectState<AcFsMuFlatfish>
{
public:
    AcFsMuFlatfish(); // ctor address unknown
    virtual ~AcFsMuFlatfish(); // ModuleMusFish.cro +0x0103B4 slot 0x00
};
