#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x20670 in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x20730 in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x20764 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x2079C in ModuleMusIns.cro, offset_to_top -720, 11 entries
class AcIsMuFeather : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuFeather>
{
public:
    AcIsMuFeather(); // ctor address unknown
    virtual ~AcIsMuFeather(); // ModuleMusIns.cro +0x005F3C slot 0x00
};
