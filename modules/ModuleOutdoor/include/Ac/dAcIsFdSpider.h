#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x93510 in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x935FC in ModuleOutdoor.cro, offset_to_top -464, 9 entries
// vtable +0x93628 in ModuleOutdoor.cro, offset_to_top -536, 2 entries
// vtable +0x93660 in ModuleOutdoor.cro, offset_to_top -588, 11 entries
class AcIsFdSpider : public ::AcInsectFieldBase, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdSpider>
{
public:
    AcIsFdSpider(); // ctor address unknown
    virtual ~AcIsFdSpider(); // ModuleOutdoor.cro +0x012450 slot 0x00
    virtual void Draw(); // ModuleOutdoor.cro +0x0121E4 slot 0x30
};
