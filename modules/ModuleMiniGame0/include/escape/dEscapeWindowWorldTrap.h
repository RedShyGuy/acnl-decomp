#pragma once

#include "decomp.h"
#include "escape/dEscapeWindowWorldWithDice.h"

namespace escape {
// vtable +0xE1488 in ModuleMiniGame0.cro, offset_to_top 0, 52 entries
class EscapeWindowWorldTrap : public ::escape::EscapeWindowWorldWithDice
{
public:
    EscapeWindowWorldTrap(); // ctor address unknown
};
} // namespace escape
