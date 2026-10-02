#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xDFF88 in ModuleMiniGame0.cro, offset_to_top 0, 23 entries
class BsFreeHexSelector : public ::UtlBase<Base>
{
public:
    BsFreeHexSelector(); // ctor address unknown
    virtual ~BsFreeHexSelector(); // ModuleMiniGame0.cro +0x045ED8 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x045CE0 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x0458A4 slot 0x30
};
} // namespace escape
