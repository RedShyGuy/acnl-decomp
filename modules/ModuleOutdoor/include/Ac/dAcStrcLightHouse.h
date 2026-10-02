#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x95EC0 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcLightHouse : public ::UtlBase<AcStrc>
{
public:
    AcStrcLightHouse(); // ctor address unknown
    virtual ~AcStrcLightHouse(); // ModuleOutdoor.cro +0x037264 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x036F70 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x036ED4 slot 0x30
};
