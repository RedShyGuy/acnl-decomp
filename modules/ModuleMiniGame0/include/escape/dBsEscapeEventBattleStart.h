#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE237C in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventBattleStart : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventBattleStart(); // ctor address unknown
    virtual ~BsEscapeEventBattleStart(); // ModuleMiniGame0.cro +0x096D28 slot 0x00
};
} // namespace escape
