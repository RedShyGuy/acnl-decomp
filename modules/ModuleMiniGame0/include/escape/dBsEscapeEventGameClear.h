#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE1778 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventGameClear : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventGameClear(); // ctor address unknown
    virtual ~BsEscapeEventGameClear(); // ModuleMiniGame0.cro +0x08BBFC slot 0x00
};
} // namespace escape
