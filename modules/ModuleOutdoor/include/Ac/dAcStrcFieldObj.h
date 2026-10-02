#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x94C48 in ModuleOutdoor.cro, offset_to_top 0, 65 entries
class AcStrcFieldObj : public ::UtlBase<AcStrc>
{
public:
    AcStrcFieldObj(); // ctor address unknown
    virtual ~AcStrcFieldObj(); // ModuleOutdoor.cro +0x029058 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x028B60 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x028B2C slot 0x30
};
