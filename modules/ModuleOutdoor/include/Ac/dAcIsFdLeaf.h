#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldRestless.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x91D1C in ModuleOutdoor.cro, offset_to_top 0, 57 entries
// vtable +0x91E0C in ModuleOutdoor.cro, offset_to_top -476, 9 entries
// vtable +0x91E38 in ModuleOutdoor.cro, offset_to_top -548, 2 entries
// vtable +0x91E70 in ModuleOutdoor.cro, offset_to_top -604, 11 entries
class AcIsFdLeaf : public ::AcInsectFieldRestless, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdLeaf>
{
public:
    AcIsFdLeaf(); // ctor address unknown
    virtual ~AcIsFdLeaf(); // ModuleOutdoor.cro +0x004334 slot 0x00
};
