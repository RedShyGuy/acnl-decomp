#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x9627C in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x96368 in ModuleOutdoor.cro, offset_to_top -464, 9 entries
// vtable +0x96394 in ModuleOutdoor.cro, offset_to_top -536, 2 entries
// vtable +0x963CC in ModuleOutdoor.cro, offset_to_top -552, 11 entries
class AcIsFdCastOffSkin : public ::AcInsectFieldBase, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdCastOffSkin>
{
public:
    AcIsFdCastOffSkin(); // ctor address unknown
    virtual ~AcIsFdCastOffSkin(); // ModuleOutdoor.cro +0x04EB1C slot 0x00
    virtual void Draw(); // ModuleOutdoor.cro +0x04E7A0 slot 0x30
};
