#pragma once

#include "decomp.h"
#include "escape/dEscapeWindowWorldBattle.h"

namespace escape {
// vtable +0xE3160 in ModuleMiniGame0.cro, offset_to_top 0, 52 entries
class EscapeWindowWorldWithDice : public ::escape::EscapeWindowWorldBattle
{
public:
    EscapeWindowWorldWithDice(); // ctor address unknown
};
} // namespace escape
