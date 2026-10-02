#pragma once

#include "decomp.h"
#include "esc/dBsEscModelBase.h"

namespace escape {
// vtable +0xE0788 in ModuleMiniGame0.cro, offset_to_top 0, 35 entries
class BsEscapeModelCursor : public ::esc::BsEscModelBase
{
public:
    BsEscapeModelCursor(); // ctor address unknown
    virtual ~BsEscapeModelCursor(); // ModuleMiniGame0.cro +0x07E07C slot 0x00
};
} // namespace escape
