#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumRiver.h"
#include "Object/dObjectState.h"

// vtable +0x2FA74 in ModuleMusFish.cro, offset_to_top 0, 48 entries
// vtable +0x2FB40 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2FB74 in ModuleMusFish.cro, offset_to_top -596, 2 entries
// vtable +0x2FBAC in ModuleMusFish.cro, offset_to_top -768, 11 entries
class AcFsMuSoftShelledTurtle : public ::AcFishMuseumRiver, public ::ObjectState<AcFsMuSoftShelledTurtle>
{
public:
    AcFsMuSoftShelledTurtle(); // ctor address unknown
    virtual ~AcFsMuSoftShelledTurtle(); // ModuleMusFish.cro +0x00EA30 slot 0x00
};
