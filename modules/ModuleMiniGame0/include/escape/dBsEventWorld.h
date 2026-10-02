#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xDEFE4 in ModuleMiniGame0.cro, offset_to_top 0, 22 entries
class BsEventWorld : public ::UtlBase<Base>
{
public:
    BsEventWorld(); // ctor address unknown
    virtual ~BsEventWorld(); // ModuleMiniGame0.cro +0x01C988 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x01C73C slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x01C228 slot 0x30
};
} // namespace escape
