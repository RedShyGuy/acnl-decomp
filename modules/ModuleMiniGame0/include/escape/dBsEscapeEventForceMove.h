#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE16D4 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventForceMove : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventForceMove(); // ctor address unknown
    virtual ~BsEscapeEventForceMove(); // ModuleMiniGame0.cro +0x08B9AC slot 0x00
};
} // namespace escape
