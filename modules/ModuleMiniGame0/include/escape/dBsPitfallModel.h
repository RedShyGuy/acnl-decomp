#pragma once

#include "decomp.h"
#include "esc/dBsEscModelBase.h"

namespace escape {
// vtable +0xDF37C in ModuleMiniGame0.cro, offset_to_top 0, 35 entries
class BsPitfallModel : public ::esc::BsEscModelBase
{
public:
    BsPitfallModel(); // ctor address unknown
    virtual ~BsPitfallModel(); // ModuleMiniGame0.cro +0x02B23C slot 0x00
};
} // namespace escape
