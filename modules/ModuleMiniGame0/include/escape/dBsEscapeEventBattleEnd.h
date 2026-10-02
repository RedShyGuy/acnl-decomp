#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE1628 in ModuleMiniGame0.cro, offset_to_top 0, 41 entries
class BsEscapeEventBattleEnd : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventBattleEnd(); // ctor address unknown
    virtual ~BsEscapeEventBattleEnd(); // ModuleMiniGame0.cro +0x08B698 slot 0x00
};
} // namespace escape
