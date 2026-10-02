#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2F180 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2F244 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2F278 in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2F2B0 in ModuleMusFish.cro, offset_to_top -744, 11 entries
class AcFsMuSeaMollusc : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuSeaMollusc>
{
public:
    AcFsMuSeaMollusc(); // ctor address unknown
    virtual ~AcFsMuSeaMollusc(); // ModuleMusFish.cro +0x014F4C slot 0x00
    virtual void CanCalc() const; // ModuleMusFish.cro +0x0148F4 slot 0x20
};
