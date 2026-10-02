#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE0238 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventCheck : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventCheck(); // ctor address unknown
    virtual ~BsEscapeEventCheck(); // ModuleMiniGame0.cro +0x0798F4 slot 0x00
};
} // namespace escape
