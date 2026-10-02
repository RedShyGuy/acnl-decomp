#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE0C08 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventTurnEnd : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventTurnEnd(); // ctor address unknown
    virtual ~BsEscapeEventTurnEnd(); // ModuleMiniGame0.cro +0x081600 slot 0x00
};
} // namespace escape
