#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE1CC8 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventBeforeMove : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventBeforeMove(); // ctor address unknown
    virtual ~BsEscapeEventBeforeMove(); // ModuleMiniGame0.cro +0x0938E4 slot 0x00
};
} // namespace escape
