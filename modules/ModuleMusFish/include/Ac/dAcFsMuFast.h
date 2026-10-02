#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumRiver.h"
#include "Object/dObjectState.h"

// vtable +0x2C4E0 in ModuleMusFish.cro, offset_to_top 0, 48 entries
// vtable +0x2C5AC in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2C5E0 in ModuleMusFish.cro, offset_to_top -596, 2 entries
// vtable +0x2C618 in ModuleMusFish.cro, offset_to_top -780, 11 entries
class AcFsMuFast : public ::AcFishMuseumRiver, public ::ObjectState<AcFsMuFast>
{
public:
    AcFsMuFast(); // ctor address unknown
    virtual ~AcFsMuFast(); // ModuleMusFish.cro +0x012060 slot 0x00
};
