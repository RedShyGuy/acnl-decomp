#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2E8F4 in ModuleMusFish.cro, offset_to_top 0, 47 entries
// vtable +0x2E9BC in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2E9F0 in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2EA28 in ModuleMusFish.cro, offset_to_top -572, 11 entries
class AcFsMuSeaSimple : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuSeaSimple>
{
public:
    AcFsMuSeaSimple(); // ctor address unknown
    virtual ~AcFsMuSeaSimple(); // ModuleMusFish.cro +0x00C540 slot 0x00
    virtual void CanCalc() const; // ModuleMusFish.cro +0x00C0A4 slot 0x20
};
