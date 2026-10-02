#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumSea.h"
#include "Object/dObjectState.h"

// vtable +0x2D400 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2D4C4 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2D4F8 in ModuleMusFish.cro, offset_to_top -632, 2 entries
// vtable +0x2D530 in ModuleMusFish.cro, offset_to_top -820, 11 entries
class AcFsMuOarfish : public ::AcFishMuseumSea, public ::ObjectState<AcFsMuOarfish>
{
public:
    AcFsMuOarfish(); // ctor address unknown
    virtual ~AcFsMuOarfish(); // ModuleMusFish.cro +0x010E78 slot 0x00
};
