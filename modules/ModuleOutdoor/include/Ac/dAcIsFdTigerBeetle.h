#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldRestless.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x967C4 in ModuleOutdoor.cro, offset_to_top 0, 57 entries
// vtable +0x968B4 in ModuleOutdoor.cro, offset_to_top -476, 9 entries
// vtable +0x968E0 in ModuleOutdoor.cro, offset_to_top -548, 2 entries
// vtable +0x96918 in ModuleOutdoor.cro, offset_to_top -608, 11 entries
class AcIsFdTigerBeetle : public ::AcInsectFieldRestless, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdTigerBeetle>
{
public:
    AcIsFdTigerBeetle(); // ctor address unknown
    virtual ~AcIsFdTigerBeetle(); // ModuleOutdoor.cro +0x051DC8 slot 0x00
};
