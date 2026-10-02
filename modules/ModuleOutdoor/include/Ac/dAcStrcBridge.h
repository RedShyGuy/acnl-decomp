#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x936C8 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcBridge : public ::UtlBase<AcStrc>
{
public:
    AcStrcBridge(); // ctor address unknown
    virtual ~AcStrcBridge(); // ModuleOutdoor.cro +0x013BC0 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x0139C8 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x013958 slot 0x30
};
