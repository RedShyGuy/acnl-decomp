#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xDFE50 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventWait : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventWait(); // ctor address unknown
    virtual ~BsEscapeEventWait(); // ModuleMiniGame0.cro +0x04520C slot 0x00
};
} // namespace escape
