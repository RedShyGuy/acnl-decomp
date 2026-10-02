#pragma once

#include "decomp.h"
#include "Ac/dAcInsectFieldBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklVis.h"

// vtable +0x9550C in ModuleOutdoor.cro, offset_to_top 0, 56 entries
// vtable +0x955F8 in ModuleOutdoor.cro, offset_to_top -464, 9 entries
// vtable +0x95624 in ModuleOutdoor.cro, offset_to_top -536, 2 entries
// vtable +0x9565C in ModuleOutdoor.cro, offset_to_top -636, 11 entries
class AcIsFdTumblebug : public ::AcInsectFieldBase, public ::ResourceGetSklVis, public ::ObjectState<AcIsFdTumblebug>
{
public:
    AcIsFdTumblebug(); // ctor address unknown
    virtual ~AcIsFdTumblebug(); // ModuleOutdoor.cro +0x030CA8 slot 0x00
};
