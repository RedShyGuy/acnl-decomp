#pragma once

#include "decomp.h"
#include "esc/dBsEscModelBase.h"

namespace escape {
// vtable +0xDEDB0 in ModuleMiniGame0.cro, offset_to_top 0, 35 entries
class BsBoatModel : public ::esc::BsEscModelBase
{
public:
    BsBoatModel(); // ctor address unknown
    virtual ~BsBoatModel(); // ModuleMiniGame0.cro +0x0142C8 slot 0x00
};
} // namespace escape
