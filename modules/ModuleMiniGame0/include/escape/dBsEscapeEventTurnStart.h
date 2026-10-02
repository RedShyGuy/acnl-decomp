#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE181C in ModuleMiniGame0.cro, offset_to_top 0, 42 entries
class BsEscapeEventTurnStart : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventTurnStart(); // ctor address unknown
    virtual ~BsEscapeEventTurnStart(); // ModuleMiniGame0.cro +0x08CE50 slot 0x00
};
} // namespace escape
