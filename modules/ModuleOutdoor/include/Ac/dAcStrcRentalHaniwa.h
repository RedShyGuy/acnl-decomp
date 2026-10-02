#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x97214 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcRentalHaniwa : public ::UtlBase<AcStrc>
{
public:
    AcStrcRentalHaniwa(); // ctor address unknown
    virtual ~AcStrcRentalHaniwa(); // ModuleOutdoor.cro +0x05ACC4 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x05AA8C slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x05AA4C slot 0x30
};
