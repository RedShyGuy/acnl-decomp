#pragma once

#include "decomp.h"
#include "escape/dEscapeWindowWorldBattle.h"

namespace escape {
// vtable +0xE1B10 in ModuleMiniGame0.cro, offset_to_top 0, 54 entries
class EscapeWindowWorldCatch : public ::escape::EscapeWindowWorldBattle
{
public:
    EscapeWindowWorldCatch(); // ctor address unknown
};
} // namespace escape
