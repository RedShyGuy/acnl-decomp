#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x211C4 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x21284 in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x212B8 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x212F0 in ModuleMusIns.cro, offset_to_top -824, 11 entries
class AcIsMuDragonfly : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuDragonfly>
{
public:
    AcIsMuDragonfly(); // ctor address unknown
    virtual ~AcIsMuDragonfly(); // ModuleMusIns.cro +0x00C584 slot 0x00
};
