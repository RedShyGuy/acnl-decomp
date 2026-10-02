#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE0F30 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventCampMenu : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventCampMenu(); // ctor address unknown
    virtual ~BsEscapeEventCampMenu(); // ModuleMiniGame0.cro +0x08331C slot 0x00
};
} // namespace escape
