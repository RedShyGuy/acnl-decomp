#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x96B4C in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcDonationBox : public ::UtlBase<AcStrc>
{
public:
    AcStrcDonationBox(); // ctor address unknown
    virtual ~AcStrcDonationBox(); // ModuleOutdoor.cro +0x0538BC slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x0535AC slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x053588 slot 0x30
};
