#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x2184C in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x2190C in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x21940 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x21978 in ModuleMusIns.cro, offset_to_top -724, 11 entries
class AcIsMuCastOffSkin : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuCastOffSkin>
{
public:
    AcIsMuCastOffSkin(); // ctor address unknown
    virtual ~AcIsMuCastOffSkin(); // ModuleMusIns.cro +0x00E034 slot 0x00
};
