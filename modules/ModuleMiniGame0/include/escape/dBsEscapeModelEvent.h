#pragma once

#include "decomp.h"
#include "esc/dBsEscModelBase.h"

namespace escape {
// vtable +0xE02DC in ModuleMiniGame0.cro, offset_to_top 0, 35 entries
class BsEscapeModelEvent : public ::esc::BsEscModelBase
{
public:
    BsEscapeModelEvent(); // ctor address unknown
    virtual ~BsEscapeModelEvent(); // ModuleMiniGame0.cro +0x07998C slot 0x00
};
} // namespace escape
