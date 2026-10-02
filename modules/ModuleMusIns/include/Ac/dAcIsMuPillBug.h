#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceGetSklMat.h"

// vtable +0x209A8 in ModuleMusIns.cro, offset_to_top 0, 48 entries
// vtable +0x20A74 in ModuleMusIns.cro, offset_to_top -396, 9 entries
// vtable +0x20AA0 in ModuleMusIns.cro, offset_to_top -476, 2 entries
// vtable +0x20AD8 in ModuleMusIns.cro, offset_to_top -788, 11 entries
class AcIsMuPillBug : public ::AcInsectMuseumBase, public ::ResourceGetSklMat, public ::ObjectState<AcIsMuPillBug>
{
public:
    AcIsMuPillBug(); // ctor address unknown
    virtual ~AcIsMuPillBug(); // ModuleMusIns.cro +0x006F50 slot 0x00
};
