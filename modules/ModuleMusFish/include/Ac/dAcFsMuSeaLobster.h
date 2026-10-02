#pragma once

#include "decomp.h"
#include "Ac/dAcFishMuseumBase.h"
#include "Object/dObjectState.h"

// vtable +0x2EFD8 in ModuleMusFish.cro, offset_to_top 0, 46 entries
// vtable +0x2F09C in ModuleMusFish.cro, offset_to_top -208, 11 entries
// vtable +0x2F0D0 in ModuleMusFish.cro, offset_to_top -552, 2 entries
// vtable +0x2F108 in ModuleMusFish.cro, offset_to_top -736, 11 entries
class AcFsMuSeaLobster : public ::AcFishMuseumBase, public ::ObjectState<AcFsMuSeaLobster>
{
public:
    AcFsMuSeaLobster(); // ctor address unknown
    virtual ~AcFsMuSeaLobster(); // ModuleMusFish.cro +0x0148A8 slot 0x00
};
