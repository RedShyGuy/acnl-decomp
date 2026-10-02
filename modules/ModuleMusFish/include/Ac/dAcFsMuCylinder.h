#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2DCDC in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2DDA0 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2DDD4 in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2DE0C in ModuleMusFish.cro, offset_to_top -744, 11 entries
class AcFsMuCylinder : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuCylinder>
{
public:
    AcFsMuCylinder(); // ctor address unknown
    virtual ~AcFsMuCylinder(); // ModuleMusFish.cro +0x009820 slot 0x00
};
