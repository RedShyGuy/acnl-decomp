#pragma once

#include "decomp.h"
#include "escape/dEscapeWindowWorldBattle.h"

namespace escape {
// vtable +0xE2E1C in ModuleMiniGame0.cro, offset_to_top 0, 52 entries
class EscapeWindowWorldCampOnly : public ::escape::EscapeWindowWorldBattle
{
public:
    EscapeWindowWorldCampOnly(); // ctor address unknown
};
} // namespace escape
