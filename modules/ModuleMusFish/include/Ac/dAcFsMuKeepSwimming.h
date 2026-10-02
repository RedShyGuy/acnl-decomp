#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2F824 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2F8E8 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2F91C in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2F954 in ModuleMusFish.cro, offset_to_top -768, 11 entries
class AcFsMuKeepSwimming : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuKeepSwimming>
{
public:
    AcFsMuKeepSwimming(); // ctor address unknown
    virtual ~AcFsMuKeepSwimming(); // ModuleMusFish.cro +0x01677C slot 0x00
};
