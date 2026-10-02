#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x939B0 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcScreen : public ::UtlBase<AcStrc>
{
public:
    AcStrcScreen(); // ctor address unknown
    virtual ~AcStrcScreen(); // ModuleOutdoor.cro +0x0173C8 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x0172DC slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x01725C slot 0x30
};
