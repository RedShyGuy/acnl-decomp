#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE37E0 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventDispConditions : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventDispConditions(); // ctor address unknown
    virtual ~BsEscapeEventDispConditions(); // ModuleMiniGame0.cro +0x09F7E4 slot 0x00
};
} // namespace escape
