#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x216AC in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x2176C in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x217A0 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x217D8 in ModuleMusIns.cro, offset_to_top -744, 11 entries
class AcIsMuHermitCrab : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuHermitCrab>
{
public:
    AcIsMuHermitCrab(); // ctor address unknown
    virtual ~AcIsMuHermitCrab(); // ModuleMusIns.cro +0x00DCDC slot 0x00
};
