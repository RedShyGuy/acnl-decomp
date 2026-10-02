#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2D918 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2D9DC in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2DA10 in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2DA48 in ModuleMusFish.cro, offset_to_top -760, 11 entries
class AcFsMuSunfish : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuSunfish>
{
public:
    AcFsMuSunfish(); // ctor address unknown
    virtual ~AcFsMuSunfish(); // ModuleMusFish.cro +0x007798 slot 0x00
};
