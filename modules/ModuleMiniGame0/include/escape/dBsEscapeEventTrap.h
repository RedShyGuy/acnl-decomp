#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xDFDA4 in ModuleMiniGame0.cro, offset_to_top 0, 41 entries
class BsEscapeEventTrap : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventTrap(); // ctor address unknown
    virtual ~BsEscapeEventTrap(); // ModuleMiniGame0.cro +0x044FD0 slot 0x00
};
} // namespace escape
