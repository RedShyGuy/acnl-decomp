#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2D0C0 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2D184 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2D1B8 in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2D1F0 in ModuleMusFish.cro, offset_to_top -744, 11 entries
class AcFsMuWander : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuWander>
{
public:
    AcFsMuWander(); // ctor address unknown
    virtual ~AcFsMuWander(); // ModuleMusFish.cro +0x004B90 slot 0x00
};
