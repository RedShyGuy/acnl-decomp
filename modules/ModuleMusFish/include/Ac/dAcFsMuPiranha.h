#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumRiver.h"
#include "Object/dObjectState.h"

// vtable +0x2D5B4 in ModuleMusFish.cro, offset_to_top 0, 48 entries
// vtable +0x2D680 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2D6B4 in ModuleMusFish.cro, offset_to_top -596, 2 entries
// vtable +0x2D6EC in ModuleMusFish.cro, offset_to_top -896, 11 entries
class AcFsMuPiranha : public ::AcFishMuseumRiver, public ::ObjectState<AcFsMuPiranha>
{
public:
    AcFsMuPiranha(); // ctor address unknown
    virtual ~AcFsMuPiranha(); // ModuleMusFish.cro +0x010CA8 slot 0x00
};
