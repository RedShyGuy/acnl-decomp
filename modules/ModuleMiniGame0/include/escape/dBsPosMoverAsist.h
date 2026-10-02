#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xDF71C in ModuleMiniGame0.cro, offset_to_top 0, 23 entries
class BsPosMoverAsist : public ::UtlBase<Base>
{
public:
    BsPosMoverAsist(); // ctor address unknown
    virtual ~BsPosMoverAsist(); // ModuleMiniGame0.cro +0x033DAC slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x033C6C slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x033C10 slot 0x30
};
} // namespace escape
