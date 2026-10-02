#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x20CE4 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x20DA4 in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x20DD8 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x20E10 in ModuleMusIns.cro, offset_to_top -760, 11 entries
class AcIsMuMosquito : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuMosquito>
{
public:
    AcIsMuMosquito(); // ctor address unknown
    virtual ~AcIsMuMosquito(); // ModuleMusIns.cro +0x007EC0 slot 0x00
};
