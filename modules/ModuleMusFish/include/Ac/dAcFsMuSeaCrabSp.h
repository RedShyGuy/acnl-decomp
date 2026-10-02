#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2E5A4 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2E668 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2E69C in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2E6D4 in ModuleMusFish.cro, offset_to_top -576, 11 entries
class AcFsMuSeaCrabSp : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuSeaCrabSp>
{
public:
    AcFsMuSeaCrabSp(); // ctor address unknown
    virtual ~AcFsMuSeaCrabSp(); // ModuleMusFish.cro +0x00B7E4 slot 0x00
};
