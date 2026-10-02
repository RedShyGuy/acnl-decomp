#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x92C3C in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x92D28 in ModuleOutdoor.cro, offset_to_top -464, 9 entries
// vtable +0x92D54 in ModuleOutdoor.cro, offset_to_top -536, 2 entries
// vtable +0x92D8C in ModuleOutdoor.cro, offset_to_top -568, 11 entries
class AcIsFdBeetle : public ::AcInsectFieldBase, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdBeetle>
{
public:
    AcIsFdBeetle(); // ctor address unknown
    virtual ~AcIsFdBeetle(); // ModuleOutdoor.cro +0x00DBEC slot 0x00
    virtual void Draw(); // ModuleOutdoor.cro +0x00D8C0 slot 0x30
};
