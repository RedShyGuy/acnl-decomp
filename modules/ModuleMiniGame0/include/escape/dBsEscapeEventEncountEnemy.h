#pragma once

#include "decomp.h"
#include "escape/dBsEscapeEventBase.h"

namespace escape {
// vtable +0xE2B44 in ModuleMiniGame0.cro, offset_to_top 0, 39 entries
class BsEscapeEventEncountEnemy : public ::escape::BsEscapeEventBase
{
public:
    BsEscapeEventEncountEnemy(); // ctor address unknown
    virtual ~BsEscapeEventEncountEnemy(); // ModuleMiniGame0.cro +0x099D30 slot 0x00
};
} // namespace escape
