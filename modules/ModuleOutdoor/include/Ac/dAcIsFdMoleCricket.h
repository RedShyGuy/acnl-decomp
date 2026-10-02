#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldRestless.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x96438 in ModuleOutdoor.cro, offset_to_top 0, 57 entries
// vtable +0x96528 in ModuleOutdoor.cro, offset_to_top -476, 9 entries
// vtable +0x96554 in ModuleOutdoor.cro, offset_to_top -548, 2 entries
// vtable +0x9658C in ModuleOutdoor.cro, offset_to_top -608, 11 entries
class AcIsFdMoleCricket : public ::AcInsectFieldRestless, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdMoleCricket>
{
public:
    AcIsFdMoleCricket(); // ctor address unknown
    virtual ~AcIsFdMoleCricket(); // ModuleOutdoor.cro +0x04FDF0 slot 0x00
    virtual void Draw(); // ModuleOutdoor.cro +0x04FB80 slot 0x30
};
