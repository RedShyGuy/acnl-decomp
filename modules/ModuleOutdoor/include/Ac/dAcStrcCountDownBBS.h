#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x97024 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcCountDownBBS : public ::UtlBase<AcStrc>
{
public:
    AcStrcCountDownBBS(); // ctor address unknown
    virtual ~AcStrcCountDownBBS(); // ModuleOutdoor.cro +0x059228 slot 0x00
    virtual oml::framework::Result Calc(); // ModuleOutdoor.cro +0x058B88 slot 0x24
    virtual oml::framework::Result Draw(); // ModuleOutdoor.cro +0x058B10 slot 0x30
};
