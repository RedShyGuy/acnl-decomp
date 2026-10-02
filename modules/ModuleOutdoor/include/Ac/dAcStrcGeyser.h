#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x937C0 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcGeyser : public ::UtlBase<AcStrc>
{
public:
    AcStrcGeyser(); // ctor address unknown
    virtual ~AcStrcGeyser(); // ModuleOutdoor.cro +0x014C1C slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x014A1C slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x014990 slot 0x30
};
