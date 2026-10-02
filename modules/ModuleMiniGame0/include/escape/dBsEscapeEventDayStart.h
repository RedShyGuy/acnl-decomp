#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE0FD4 in ModuleMiniGame0.cro, offset_to_top 0, 40 entries
class BsEscapeEventDayStart : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventDayStart(); // ctor address unknown
    virtual ~BsEscapeEventDayStart(); // ModuleMiniGame0.cro +0x083F54 slot 0x00
};
} // namespace escape
