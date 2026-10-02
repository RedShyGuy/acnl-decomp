#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x1F458 in ModuleMusIns.cro, offset_to_top 0, 46 entries
// vtable +0x1F51C in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x1F550 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x1F588 in ModuleMusIns.cro, offset_to_top -760, 11 entries
class AcIsMuFlea : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuFlea>
{
public:
    AcIsMuFlea(); // ctor address unknown
    virtual ~AcIsMuFlea(); // ModuleMusIns.cro +0x000A2C slot 0x00
};
