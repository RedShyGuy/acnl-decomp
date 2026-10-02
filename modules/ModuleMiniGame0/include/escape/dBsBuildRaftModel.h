#pragma once

#include "decomp.h"
#include "esc/dBsEscModelBase.h"

namespace escape {
// vtable +0xDF8C4 in ModuleMiniGame0.cro, offset_to_top 0, 35 entries
class BsBuildRaftModel : public ::esc::BsEscModelBase
{
public:
    BsBuildRaftModel(); // ctor address unknown
    virtual ~BsBuildRaftModel(); // ModuleMiniGame0.cro +0x034D10 slot 0x00
};
} // namespace escape
