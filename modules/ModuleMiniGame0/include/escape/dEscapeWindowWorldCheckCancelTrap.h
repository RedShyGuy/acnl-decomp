#pragma once

#include "decomp.h"
#include "escape/dEscapeWindowWorldNoEventWindow.h"

namespace escape {
// vtable +0xE49EC in ModuleMiniGame0.cro, offset_to_top 0, 52 entries
class EscapeWindowWorldCheckCancelTrap : public ::escape::EscapeWindowWorldNoEventWindow
{
public:
    EscapeWindowWorldCheckCancelTrap(); // ctor address unknown
};
} // namespace escape
