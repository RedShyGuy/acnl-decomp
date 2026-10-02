#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

namespace resetsanin {
// vtable +0x6988 in ModuleReMoCnt.cro, offset_to_top 0, 1 entries
class GetRacketsanFunction : public ::BsNpcMgr::NpcForeachFunction
{
public:
    GetRacketsanFunction(); // ctor address unknown
};
} // namespace resetsanin
