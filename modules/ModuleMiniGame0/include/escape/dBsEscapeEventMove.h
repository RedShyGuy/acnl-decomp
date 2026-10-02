#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xDFC5C in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventMove : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventMove(); // ctor address unknown
    virtual ~BsEscapeEventMove(); // ModuleMiniGame0.cro +0x043B48 slot 0x00
};
} // namespace escape
