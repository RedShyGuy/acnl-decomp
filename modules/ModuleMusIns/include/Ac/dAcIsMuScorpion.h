#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x20E84 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x20F44 in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x20F78 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x20FB0 in ModuleMusIns.cro, offset_to_top -772, 11 entries
class AcIsMuScorpion : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuScorpion>
{
public:
    AcIsMuScorpion(); // ctor address unknown
    virtual ~AcIsMuScorpion(); // ModuleMusIns.cro +0x008DA4 slot 0x00
};
