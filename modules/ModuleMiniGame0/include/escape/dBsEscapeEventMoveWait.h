#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE11C4 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventMoveWait : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventMoveWait(); // ctor address unknown
    virtual ~BsEscapeEventMoveWait(); // ModuleMiniGame0.cro +0x085B6C slot 0x00
};
} // namespace escape
