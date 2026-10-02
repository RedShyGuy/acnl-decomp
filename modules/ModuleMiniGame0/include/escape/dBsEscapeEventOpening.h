#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE0B64 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventOpening : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventOpening(); // ctor address unknown
    virtual ~BsEscapeEventOpening(); // ModuleMiniGame0.cro +0x080FC0 slot 0x00
};
} // namespace escape
