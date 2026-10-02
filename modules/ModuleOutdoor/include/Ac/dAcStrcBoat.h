#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x920A4 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcBoat : public ::UtlBase<AcStrc>
{
public:
    AcStrcBoat(); // ctor address unknown
    virtual ~AcStrcBoat(); // ModuleOutdoor.cro +0x007354 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x0065D8 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x00659C slot 0x30
};
