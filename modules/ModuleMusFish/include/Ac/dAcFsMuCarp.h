#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumRiver.h"
#include "Object/dObjectState.h"

// vtable +0x2C324 in ModuleMusFish.cro, offset_to_top 0, 48 entries
// vtable +0x2C3F0 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2C424 in ModuleMusFish.cro, offset_to_top -596, 2 entries
// vtable +0x2C45C in ModuleMusFish.cro, offset_to_top -780, 11 entries
class AcFsMuCarp : public ::AcFishMuseumRiver, public ::ObjectState<AcFsMuCarp>
{
public:
    AcFsMuCarp(); // ctor address unknown
    virtual ~AcFsMuCarp(); // ModuleMusFish.cro +0x012364 slot 0x00
};
