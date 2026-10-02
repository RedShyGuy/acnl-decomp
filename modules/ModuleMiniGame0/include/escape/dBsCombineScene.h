#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xDF2BC in ModuleMiniGame0.cro, offset_to_top 0, 22 entries
class BsCombineScene : public ::UtlBase<Base>
{
public:
    BsCombineScene(); // ctor address unknown
    virtual ~BsCombineScene(); // ModuleMiniGame0.cro +0x02A454 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x02A40C slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x029CCC slot 0x30
};
} // namespace escape
