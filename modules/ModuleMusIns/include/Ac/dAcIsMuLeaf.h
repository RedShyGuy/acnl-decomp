#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x1F5FC in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1F6BC in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x1F6F0 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x1F728 in ModuleMusIns.cro, offset_to_top -744, 11 entries
class AcIsMuLeaf : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuLeaf>
{
public:
    AcIsMuLeaf(); // ctor address unknown
    virtual ~AcIsMuLeaf(); // ModuleMusIns.cro +0x000FE8 slot 0x00
};
