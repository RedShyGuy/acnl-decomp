#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE1D6C in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventFishShadow : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventFishShadow(); // ctor address unknown
    virtual ~BsEscapeEventFishShadow(); // ModuleMiniGame0.cro +0x093A98 slot 0x00
};
} // namespace escape
