#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2FDD8 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2FE9C in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2FED0 in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2FF08 in ModuleMusFish.cro, offset_to_top -708, 11 entries
class AcFsMuRay : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuRay>
{
public:
    AcFsMuRay(); // ctor address unknown
    virtual ~AcFsMuRay(); // ModuleMusFish.cro +0x01A250 slot 0x00
};
