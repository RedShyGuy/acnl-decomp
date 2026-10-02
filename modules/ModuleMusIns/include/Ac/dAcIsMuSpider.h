#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x204D0 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x20590 in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x205C4 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x205FC in ModuleMusIns.cro, offset_to_top -732, 11 entries
class AcIsMuSpider : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuSpider>
{
public:
    AcIsMuSpider(); // ctor address unknown
    virtual ~AcIsMuSpider(); // ModuleMusIns.cro +0x005CA0 slot 0x00
};
