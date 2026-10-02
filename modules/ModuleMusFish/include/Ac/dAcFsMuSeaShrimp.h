#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2E74C in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2E810 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2E844 in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2E87C in ModuleMusFish.cro, offset_to_top -748, 11 entries
class AcFsMuSeaShrimp : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuSeaShrimp>
{
public:
    AcFsMuSeaShrimp(); // ctor address unknown
    virtual ~AcFsMuSeaShrimp(); // ModuleMusFish.cro +0x00C058 slot 0x00
};
