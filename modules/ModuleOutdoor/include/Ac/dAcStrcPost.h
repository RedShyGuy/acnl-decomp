#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x92294 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcPost : public ::UtlBase<AcStrc>
{
public:
    AcStrcPost(); // ctor address unknown
    virtual ~AcStrcPost(); // ModuleOutdoor.cro +0x008C30 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x0086D0 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x008638 slot 0x30
};
