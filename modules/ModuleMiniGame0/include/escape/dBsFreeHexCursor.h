#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xDF654 in ModuleMiniGame0.cro, offset_to_top 0, 23 entries
class BsFreeHexCursor : public ::UtlBase<Base>
{
public:
    BsFreeHexCursor(); // ctor address unknown
    virtual ~BsFreeHexCursor(); // ModuleMiniGame0.cro +0x02FFBC slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x02FF28 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x02FE20 slot 0x30
};
} // namespace escape
