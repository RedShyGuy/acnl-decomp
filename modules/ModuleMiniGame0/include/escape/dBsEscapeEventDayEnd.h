#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE0684 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventDayEnd : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventDayEnd(); // ctor address unknown
    virtual ~BsEscapeEventDayEnd(); // ModuleMiniGame0.cro +0x07DF7C slot 0x00
};
} // namespace escape
