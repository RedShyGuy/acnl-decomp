#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE24C4 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventSettingTrap : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventSettingTrap(); // ctor address unknown
    virtual ~BsEscapeEventSettingTrap(); // ModuleMiniGame0.cro +0x09727C slot 0x00
};
} // namespace escape
