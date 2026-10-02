#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE0A54 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventGetItem : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventGetItem(); // ctor address unknown
    virtual ~BsEscapeEventGetItem(); // ModuleMiniGame0.cro +0x0802CC slot 0x00
};
} // namespace escape
