#pragma once

#include "decomp.h"
#include "esc/dBsEscModelBase.h"

namespace escape {
// vtable +0xDEF50 in ModuleMiniGame0.cro, offset_to_top 0, 35 entries
class BsEnemyModel : public ::esc::BsEscModelBase
{
public:
    BsEnemyModel(); // ctor address unknown
    virtual ~BsEnemyModel(); // ModuleMiniGame0.cro +0x0178B8 slot 0x00
};
} // namespace escape
