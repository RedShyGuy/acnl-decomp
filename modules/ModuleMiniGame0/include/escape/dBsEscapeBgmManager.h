#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xE01D4 in ModuleMiniGame0.cro, offset_to_top 0, 23 entries
class BsEscapeBgmManager : public ::UtlBase<Base>
{
public:
    BsEscapeBgmManager(); // ctor address unknown
    virtual ~BsEscapeBgmManager(); // ModuleMiniGame0.cro +0x079234 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x079144 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x078F58 slot 0x30
};
} // namespace escape
