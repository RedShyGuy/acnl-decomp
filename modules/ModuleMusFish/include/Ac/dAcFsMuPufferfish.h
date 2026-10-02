#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumSea.h"
#include "Object/dObjectState.h"

// vtable +0x2EE24 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2EEE8 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2EF1C in ModuleMusFish.cro, offset_to_top -632, 2 entries
// vtable +0x2EF54 in ModuleMusFish.cro, offset_to_top -820, 11 entries
class AcFsMuPufferfish : public ::AcFishMuseumSea, public ::ObjectState<AcFsMuPufferfish>
{
public:
    AcFsMuPufferfish(); // ctor address unknown
    virtual ~AcFsMuPufferfish(); // ModuleMusFish.cro +0x00F718 slot 0x00
};
