#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2C9EC in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2CAB0 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2CAE4 in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2CB1C in ModuleMusFish.cro, offset_to_top -696, 11 entries
class AcFsMuMoray : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuMoray>
{
public:
    AcFsMuMoray(); // ctor address unknown
    virtual ~AcFsMuMoray(); // ModuleMusFish.cro +0x002EDC slot 0x00
};
