#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xDF18C in ModuleMiniGame0.cro, offset_to_top 0, 22 entries
class BsGamepadMenu : public ::UtlBase<Base>
{
public:
    BsGamepadMenu(); // ctor address unknown
    virtual ~BsGamepadMenu(); // ModuleMiniGame0.cro +0x028EE8 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x028EA4 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x028E98 slot 0x30
};
} // namespace escape
