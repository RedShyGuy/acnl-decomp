#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldRestless.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x9406C in ModuleOutdoor.cro, offset_to_top 0, 57 entries
// vtable +0x9415C in ModuleOutdoor.cro, offset_to_top -476, 9 entries
// vtable +0x94188 in ModuleOutdoor.cro, offset_to_top -548, 2 entries
// vtable +0x941C0 in ModuleOutdoor.cro, offset_to_top -608, 11 entries
class AcIsFdPillBug : public ::AcInsectFieldRestless, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdPillBug>
{
public:
    AcIsFdPillBug(); // ctor address unknown
    virtual ~AcIsFdPillBug(); // ModuleOutdoor.cro +0x01B4E4 slot 0x00
    virtual void Draw(); // ModuleOutdoor.cro +0x01B24C slot 0x30
};
