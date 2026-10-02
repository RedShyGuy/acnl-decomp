#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x94D54 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcNpcHouse : public ::UtlBase<AcStrc>
{
public:
    AcStrcNpcHouse(); // ctor address unknown
    virtual ~AcStrcNpcHouse(); // ModuleOutdoor.cro +0x02A8C0 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x02A568 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x02A49C slot 0x30
};
