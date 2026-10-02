#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE05D4 in ModuleMiniGame0.cro, offset_to_top 0, 42 entries
class BsEscapeEventBattle : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventBattle(); // ctor address unknown
    virtual ~BsEscapeEventBattle(); // ModuleMiniGame0.cro +0x07DBA4 slot 0x00
};
} // namespace escape
