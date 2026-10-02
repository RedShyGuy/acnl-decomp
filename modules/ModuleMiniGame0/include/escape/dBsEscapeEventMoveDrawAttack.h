#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE3884 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventMoveDrawAttack : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventMoveDrawAttack(); // ctor address unknown
    virtual ~BsEscapeEventMoveDrawAttack(); // ModuleMiniGame0.cro +0x0A01C8 slot 0x00
};
} // namespace escape
