#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"
#include "bgcheck/dMoveBg.h"

// vtable +0x94230 in ModuleOutdoor.cro, offset_to_top 0, 61 entries
// vtable +0x9432C in ModuleOutdoor.cro, offset_to_top -140, 4 entries
class AcStrcBbsBird : public ::UtlBase<AcStrc>, public ::bgcheck::MoveBg
{
public:
    AcStrcBbsBird(); // ctor address unknown
    virtual ~AcStrcBbsBird(); // ModuleOutdoor.cro +0x01D7C8 slot 0x00
    virtual void Calc(); // ModuleOutdoor.cro +0x01D40C slot 0x24
    virtual void Draw(); // ModuleOutdoor.cro +0x01D3E8 slot 0x30
};
