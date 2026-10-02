#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x91B60 in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x91C4C in ModuleOutdoor.cro, offset_to_top -464, 9 entries
// vtable +0x91C78 in ModuleOutdoor.cro, offset_to_top -536, 2 entries
// vtable +0x91CB0 in ModuleOutdoor.cro, offset_to_top -556, 11 entries
class AcIsFdFlea : public ::AcInsectFieldBase, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdFlea>
{
public:
    AcIsFdFlea(); // ctor address unknown
    virtual ~AcIsFdFlea(); // ModuleOutdoor.cro +0x003800 slot 0x00
};
