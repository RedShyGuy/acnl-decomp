#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldRestless.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x95CFC in ModuleOutdoor.cro, offset_to_top 0, 57 entries
// vtable +0x95DEC in ModuleOutdoor.cro, offset_to_top -476, 9 entries
// vtable +0x95E18 in ModuleOutdoor.cro, offset_to_top -548, 2 entries
// vtable +0x95E50 in ModuleOutdoor.cro, offset_to_top -604, 11 entries
class AcIsFdHermitCrab : public ::AcInsectFieldRestless, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdHermitCrab>
{
public:
    AcIsFdHermitCrab(); // ctor address unknown
    virtual ~AcIsFdHermitCrab(); // ModuleOutdoor.cro +0x036124 slot 0x00
};
