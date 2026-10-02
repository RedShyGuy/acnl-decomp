#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE3238 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventSelectMoveDir : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventSelectMoveDir(); // ctor address unknown
    virtual ~BsEscapeEventSelectMoveDir(); // ModuleMiniGame0.cro +0x09E198 slot 0x00
};
} // namespace escape
