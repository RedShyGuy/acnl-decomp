#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

namespace racketsanin {
// vtable +0x69A0 in ModuleReMoCnt.cro, offset_to_top 0, 1 entries
class GetResetsanFunction : public ::BsNpcMgr::NpcForeachFunction
{
public:
    GetResetsanFunction(); // ctor address unknown
};
} // namespace racketsanin
