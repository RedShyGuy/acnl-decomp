#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x92558 in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x92644 in ModuleOutdoor.cro, offset_to_top -464, 9 entries
// vtable +0x92670 in ModuleOutdoor.cro, offset_to_top -536, 2 entries
// vtable +0x926A8 in ModuleOutdoor.cro, offset_to_top -576, 11 entries
class AcIsFdStump : public ::AcInsectFieldBase, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdStump>
{
public:
    AcIsFdStump(); // ctor address unknown
    virtual ~AcIsFdStump(); // ModuleOutdoor.cro +0x00AA74 slot 0x00
};
