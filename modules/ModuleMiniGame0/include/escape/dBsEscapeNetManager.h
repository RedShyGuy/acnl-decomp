#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xE0370 in ModuleMiniGame0.cro, offset_to_top 0, 23 entries
class BsEscapeNetManager : public ::UtlBase<Base>
{
public:
    BsEscapeNetManager(); // ctor address unknown
    virtual ~BsEscapeNetManager(); // ModuleMiniGame0.cro +0x07AA24 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x07A934 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x079C04 slot 0x30
};
} // namespace escape
