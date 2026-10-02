#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xE0950 in ModuleMiniGame0.cro, offset_to_top 0, 22 entries
class BsActionChainManeger : public ::UtlBase<Base>
{
public:
    BsActionChainManeger(); // ctor address unknown
    virtual ~BsActionChainManeger(); // ModuleMiniGame0.cro +0x07F24C slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x07F14C slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x07F144 slot 0x30
};
} // namespace escape
