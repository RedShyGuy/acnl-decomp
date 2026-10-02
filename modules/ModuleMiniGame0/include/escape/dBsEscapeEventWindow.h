#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xE0728 in ModuleMiniGame0.cro, offset_to_top 0, 22 entries
class BsEscapeEventWindow : public ::UtlBase<Base>
{
public:
    BsEscapeEventWindow(); // ctor address unknown
    virtual ~BsEscapeEventWindow(); // ModuleMiniGame0.cro +0x07E010 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x07DFB8 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x07DFB0 slot 0x30
};
} // namespace escape
