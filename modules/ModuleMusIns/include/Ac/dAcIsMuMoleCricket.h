#pragma once

#include "decomp.h"
#include "Ac/dAcInsectMuseumBase.h"
#include "Object/dObjectState.h"
#include "Resource/dResourceLoadAsyncSkeletal.h"

// vtable +0x219EC in ModuleMusIns.cro, offset_to_top 0, 45 entries
// vtable +0x21AAC in ModuleMusIns.cro, offset_to_top -396, 11 entries
// vtable +0x21AE0 in ModuleMusIns.cro, offset_to_top -708, 2 entries
// vtable +0x21B18 in ModuleMusIns.cro, offset_to_top -764, 11 entries
class AcIsMuMoleCricket : public ::AcInsectMuseumBase, public ::ResourceLoadAsyncSkeletal, public ::ObjectState<AcIsMuMoleCricket>
{
public:
    AcIsMuMoleCricket(); // ctor address unknown
    virtual ~AcIsMuMoleCricket(); // ModuleMusIns.cro +0x00EAB4 slot 0x00
};
