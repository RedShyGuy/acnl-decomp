#pragma once

#include "decomp.h"
#include "esc/dBsEscModelBase.h"

namespace escape {
// vtable +0xDFEF4 in ModuleMiniGame0.cro, offset_to_top 0, 35 entries
class BsEscapeStageFish : public ::esc::BsEscModelBase
{
public:
    BsEscapeStageFish(); // ctor address unknown
    virtual ~BsEscapeStageFish(); // ModuleMiniGame0.cro +0x04578C slot 0x00
};
} // namespace escape
