#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2FC30 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2FCF4 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2FD28 in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2FD60 in ModuleMusFish.cro, offset_to_top -768, 11 entries
class AcFsMuEel : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuEel>
{
public:
    AcFsMuEel(); // ctor address unknown
    virtual ~AcFsMuEel(); // ModuleMusFish.cro +0x019998 slot 0x00
};
