#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2EC74 in ModuleMusFish.cro, offset_to_top 0, 48 entries
// vtable +0x2ED40 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2ED74 in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2EDAC in ModuleMusFish.cro, offset_to_top -696, 11 entries
class AcFsMuObjColBase : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuObjColBase>
{
public:
    AcFsMuObjColBase(); // ctor address unknown
    virtual ~AcFsMuObjColBase(); // ModuleMusFish.cro +0x0133EC slot 0x00
};
