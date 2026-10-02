#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2C844 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2C908 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2C93C in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2C974 in ModuleMusFish.cro, offset_to_top -732, 11 entries
class AcFsMuFloat : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuFloat>
{
public:
    AcFsMuFloat(); // ctor address unknown
    virtual ~AcFsMuFloat(); // ModuleMusFish.cro +0x002998 slot 0x00
};
