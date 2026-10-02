#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x94B50 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcFacility : public ::UtlBase<AcStrc>
{
public:
    AcStrcFacility(); // ctor address unknown
    virtual ~AcStrcFacility(); // ModuleOutdoor.cro +0x026EA8 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x0263E4 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x026398 slot 0x30
};
