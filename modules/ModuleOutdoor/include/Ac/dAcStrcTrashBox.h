#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// vtable +0x94E4C in ModuleOutdoor.cro, offset_to_top 0, 60 entries
class AcStrcTrashBox : public ::UtlBase<AcStrc>
{
public:
    AcStrcTrashBox(); // ctor address unknown
    virtual ~AcStrcTrashBox(); // ModuleOutdoor.cro +0x02B80C slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x02B670 slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x02B5EC slot 0x30
};
