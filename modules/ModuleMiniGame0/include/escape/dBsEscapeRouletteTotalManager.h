#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xE3DDC in ModuleMiniGame0.cro, offset_to_top 0, 22 entries
class BsEscapeRouletteTotalManager : public ::UtlBase<Base>
{
public:
    BsEscapeRouletteTotalManager(); // ctor address unknown
    virtual ~BsEscapeRouletteTotalManager(); // ModuleMiniGame0.cro +0x0A3AA0 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x0A3418 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x0A3410 slot 0x30
};
} // namespace escape
