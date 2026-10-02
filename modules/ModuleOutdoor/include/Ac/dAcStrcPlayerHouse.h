#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x96C44 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcPlayerHouse : public ::UtlBase<AcStrc>
{
public:
    AcStrcPlayerHouse(); // ctor address unknown
    virtual ~AcStrcPlayerHouse(); // ModuleOutdoor.cro +0x054644 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x054318 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x0542BC slot 0x30
};
