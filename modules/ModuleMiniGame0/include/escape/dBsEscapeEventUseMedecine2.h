#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE2BE8 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventUseMedecine2 : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventUseMedecine2(); // ctor address unknown
    virtual ~BsEscapeEventUseMedecine2(); // ModuleMiniGame0.cro +0x099F70 slot 0x00
};
} // namespace escape
