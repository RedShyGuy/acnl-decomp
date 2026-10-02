#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xDFD00 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventRest : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventRest(); // ctor address unknown
    virtual ~BsEscapeEventRest(); // ModuleMiniGame0.cro +0x043EAC slot 0x00
};
} // namespace escape
