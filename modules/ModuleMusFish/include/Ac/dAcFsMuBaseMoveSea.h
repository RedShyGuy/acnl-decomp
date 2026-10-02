#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumSea.h"
#include "Object/dObjectState.h"

// vtable +0x2F670 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2F734 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2F768 in ModuleMusFish.cro, offset_to_top -632, 2 entries
// vtable +0x2F7A0 in ModuleMusFish.cro, offset_to_top -824, 11 entries
class AcFsMuBaseMoveSea : public ::AcFishMuseumSea, public ::ObjectState<AcFsMuBaseMoveSea>
{
public:
    AcFsMuBaseMoveSea(); // ctor address unknown
    virtual ~AcFsMuBaseMoveSea(); // ModuleMusFish.cro +0x00F220 slot 0x00
};
