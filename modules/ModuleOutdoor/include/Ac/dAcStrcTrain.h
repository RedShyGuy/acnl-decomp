#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x92808 in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcTrain : public ::UtlBase<AcStrc>
{
public:
    AcStrcTrain(); // ctor address unknown
    virtual ~AcStrcTrain(); // ModuleOutdoor.cro +0x00CC08 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x00C800 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x00C7DC slot 0x30
};
