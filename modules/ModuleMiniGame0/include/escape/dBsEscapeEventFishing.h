#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE09B0 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventFishing : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventFishing(); // ctor address unknown
    virtual ~BsEscapeEventFishing(); // ModuleMiniGame0.cro +0x07FA68 slot 0x00
};
} // namespace escape
