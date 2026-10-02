#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Other/dObjTalkRecept.h"
#include "Utl/dUtlBase.h"

// vtable +0x95830 in ModuleOutdoor.cro, offset_to_top 0, 62 entries
// vtable +0x95930 in ModuleOutdoor.cro, offset_to_top -140, 72 entries
// vtable +0x95A58 in ModuleOutdoor.cro, offset_to_top -264, 14 entries
class AcStrcCoolerBox : public ::UtlBase<AcStrc>, public ::ObjTalkRecept
{
public:
    AcStrcCoolerBox(); // ctor address unknown
    virtual ~AcStrcCoolerBox(); // ModuleOutdoor.cro +0x0452CC slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x034468 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x0343F8 slot 0x30
};
