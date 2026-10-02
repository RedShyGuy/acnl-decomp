#pragma once

#include "decomp.h"
#include "esc/dBsEscModelBase.h"

namespace esctitle {
// vtable +0xE52EC in ModuleMiniGame0.cro, offset_to_top 0, 35 entries
class BsEscapeTitleModel : public ::esc::BsEscModelBase
{
public:
    BsEscapeTitleModel(); // ctor address unknown
    virtual ~BsEscapeTitleModel(); // ModuleMiniGame0.cro +0x0ABAA4 slot 0x00
};
} // namespace esctitle
