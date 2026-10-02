#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2D770 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2D834 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2D868 in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2D8A0 in ModuleMusFish.cro, offset_to_top -724, 11 entries
class AcFsMuSeaCrab : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuSeaCrab>
{
public:
    AcFsMuSeaCrab(); // ctor address unknown
    virtual ~AcFsMuSeaCrab(); // ModuleMusFish.cro +0x00712C slot 0x00
    virtual void CanCalc() const; // ModuleMusFish.cro +0x0068DC slot 0x20
};
