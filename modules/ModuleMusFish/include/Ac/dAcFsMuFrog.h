#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2C69C in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2C760 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2C794 in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2C7CC in ModuleMusFish.cro, offset_to_top -812, 11 entries
class AcFsMuFrog : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuFrog>
{
public:
    AcFsMuFrog(); // ctor address unknown
    virtual ~AcFsMuFrog(); // ModuleMusFish.cro +0x011F70 slot 0x00
};
