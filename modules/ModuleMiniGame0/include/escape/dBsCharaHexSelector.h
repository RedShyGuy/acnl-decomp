#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xE0110 in ModuleMiniGame0.cro, offset_to_top 0, 23 entries
class BsCharaHexSelector : public ::UtlBase<Base>
{
public:
    BsCharaHexSelector(); // ctor address unknown
    virtual ~BsCharaHexSelector(); // ModuleMiniGame0.cro +0x077310 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x077178 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x07689C slot 0x30
};
} // namespace escape
