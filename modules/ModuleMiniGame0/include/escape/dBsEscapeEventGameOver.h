#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE107C in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventGameOver : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventGameOver(); // ctor address unknown
    virtual ~BsEscapeEventGameOver(); // ModuleMiniGame0.cro +0x084080 slot 0x00
};
} // namespace escape
