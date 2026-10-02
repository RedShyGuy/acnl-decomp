#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2F328 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2F3EC in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2F420 in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2F458 in ModuleMusFish.cro, offset_to_top -836, 11 entries
class AcFsMuSeaScallop : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuSeaScallop>
{
public:
    AcFsMuSeaScallop(); // ctor address unknown
    virtual ~AcFsMuSeaScallop(); // ModuleMusFish.cro +0x00F4FC slot 0x00
    virtual void CanCalc() const; // ModuleMusFish.cro +0x014F98 slot 0x20
};
