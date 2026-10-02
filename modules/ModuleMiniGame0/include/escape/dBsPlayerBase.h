#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xDF044 in ModuleMiniGame0.cro, offset_to_top 0, 24 entries
class BsPlayerBase : public ::UtlBase<Base>
{
public:
    BsPlayerBase(); // ctor address unknown
    virtual ~BsPlayerBase(); // ModuleMiniGame0.cro +0x023B78 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x023814 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x01E604 slot 0x30
};
} // namespace escape
