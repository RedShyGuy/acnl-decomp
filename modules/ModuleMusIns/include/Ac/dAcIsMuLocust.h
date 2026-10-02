#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x2032C in ModuleMusIns.cro, offset_to_top 0, 46 entries
// vtable +0x203F0 in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x20424 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x2045C in ModuleMusIns.cro, offset_to_top -764, 11 entries
class AcIsMuLocust : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuLocust>
{
public:
    AcIsMuLocust(); // ctor address unknown
    virtual ~AcIsMuLocust(); // ModuleMusIns.cro +0x0057F8 slot 0x00
};
