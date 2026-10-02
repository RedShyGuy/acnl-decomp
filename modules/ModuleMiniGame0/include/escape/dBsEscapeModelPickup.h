#pragma once

#include "decomp.h"
#include "esc/dBsEscModelBase.h"

namespace escape {
// vtable +0xE081C in ModuleMiniGame0.cro, offset_to_top 0, 35 entries
class BsEscapeModelPickup : public ::esc::BsEscModelBase
{
public:
    BsEscapeModelPickup(); // ctor address unknown
    virtual ~BsEscapeModelPickup(); // ModuleMiniGame0.cro +0x07E258 slot 0x00
};
} // namespace escape
