#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xDFBB8 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventCure : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventCure(); // ctor address unknown
    virtual ~BsEscapeEventCure(); // ModuleMiniGame0.cro +0x0434E0 slot 0x00
};
} // namespace escape
