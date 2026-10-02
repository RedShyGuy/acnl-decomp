#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x21024 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x210E4 in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x21118 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x21150 in ModuleMusIns.cro, offset_to_top -804, 11 entries
class AcIsMuButterfly : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuButterfly>
{
public:
    AcIsMuButterfly(); // ctor address unknown
    virtual ~AcIsMuButterfly(); // ModuleMusIns.cro +0x009780 slot 0x00
};
