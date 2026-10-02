#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2CB94 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2CC58 in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2CC8C in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2CCC4 in ModuleMusFish.cro, offset_to_top -728, 11 entries
class AcFsMuShark : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuShark>
{
public:
    AcFsMuShark(); // ctor address unknown
    virtual ~AcFsMuShark(); // ModuleMusFish.cro +0x0038A8 slot 0x00
};
