#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "script/dITalkRecept.h"
#include "state/dMode.h"

// vtable +0xF1A8 in ModuleTrain.cro, offset_to_top 0, 16 entries
// vtable +0xF1F0 in ModuleTrain.cro, offset_to_top -20, 63 entries
// vtable +0xF2F4 in ModuleTrain.cro, offset_to_top -144, 3 entries
class BsUpdateSelect : public ::Base, public ::script::ITalkRecept, public ::state::Mode<BsUpdateSelect>
{
public:
    BsUpdateSelect(); // ctor address unknown
    virtual ~BsUpdateSelect(); // ModuleTrain.cro +0x0061C8 slot 0x00
    virtual void Initialize(); // ModuleTrain.cro +0x005AFC slot 0x0C
    virtual void Finalize(); // ModuleTrain.cro +0x005EE4 slot 0x18
    virtual void Calc(); // ModuleTrain.cro +0x005E40 slot 0x24
    virtual void Draw(); // ModuleTrain.cro +0x005AE0 slot 0x30
};
