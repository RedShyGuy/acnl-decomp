#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x938B8 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcOffice : public ::UtlBase<AcStrc>
{
public:
    AcStrcOffice(); // ctor address unknown
    virtual ~AcStrcOffice(); // ModuleOutdoor.cro +0x01650C slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x015EC4 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x015E04 slot 0x30
};
