#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xE32DC in ModuleMiniGame0.cro, offset_to_top 0, 25 entries
class BsEscapeWindowWorldManager : public ::UtlBase<Base>
{
public:
    BsEscapeWindowWorldManager(); // ctor address unknown
    virtual ~BsEscapeWindowWorldManager(); // ModuleMiniGame0.cro +0x09E4D0 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x09E430 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x09E2B8 slot 0x30
};
} // namespace escape
