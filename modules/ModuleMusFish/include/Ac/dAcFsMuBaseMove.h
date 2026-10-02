#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumRiver.h"
#include "Object/dObjectState.h"

// vtable +0x2DB20 in ModuleMusFish.cro, offset_to_top 0, 48 entries
// vtable +0x2DBEC in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2DC20 in ModuleMusFish.cro, offset_to_top -596, 2 entries
// vtable +0x2DC58 in ModuleMusFish.cro, offset_to_top -788, 11 entries
class AcFsMuBaseMove : public ::AcFishMuseumRiver, public ::ObjectState<AcFsMuBaseMove>
{
public:
    AcFsMuBaseMove(); // ctor address unknown
    virtual ~AcFsMuBaseMove(); // ModuleMusFish.cro +0x01071C slot 0x00
};
