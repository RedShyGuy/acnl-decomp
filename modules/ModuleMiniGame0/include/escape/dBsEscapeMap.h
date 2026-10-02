#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xDEE44 in ModuleMiniGame0.cro, offset_to_top 0, 22 entries
class BsEscapeMap : public ::UtlBase<Base>
{
public:
    BsEscapeMap(); // ctor address unknown
    virtual ~BsEscapeMap(); // ModuleMiniGame0.cro +0x016D84 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x016944 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x016710 slot 0x30
};
} // namespace escape
