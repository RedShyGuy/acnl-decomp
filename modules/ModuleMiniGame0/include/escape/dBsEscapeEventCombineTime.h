#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE2420 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventCombineTime : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventCombineTime(); // ctor address unknown
    virtual ~BsEscapeEventCombineTime(); // ModuleMiniGame0.cro +0x096EC0 slot 0x00
};
} // namespace escape
