#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x9219C in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcPier : public ::UtlBase<AcStrc>
{
public:
    AcStrcPier(); // ctor address unknown
    virtual ~AcStrcPier(); // ModuleOutdoor.cro +0x007FC8 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x007EC8 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x007E58 slot 0x30
};
