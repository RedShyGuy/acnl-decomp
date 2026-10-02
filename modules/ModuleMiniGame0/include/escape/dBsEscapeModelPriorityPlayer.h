#pragma once

#include "decomp.h"
#include "esc/dBsEscModelBase.h"

namespace escape {
// vtable +0xE3928 in ModuleMiniGame0.cro, offset_to_top 0, 35 entries
class BsEscapeModelPriorityPlayer : public ::esc::BsEscModelBase
{
public:
    BsEscapeModelPriorityPlayer(); // ctor address unknown
    virtual ~BsEscapeModelPriorityPlayer(); // ModuleMiniGame0.cro +0x0A0294 slot 0x00
};
} // namespace escape
