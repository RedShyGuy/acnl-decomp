#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x1F93C in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x1F9FC in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x1FA30 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x1FA68 in ModuleMusIns.cro, offset_to_top -744, 11 entries
class AcIsMuStump : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuStump>
{
public:
    AcIsMuStump(); // ctor address unknown
    virtual ~AcIsMuStump(); // ModuleMusIns.cro +0x001C30 slot 0x00
};
