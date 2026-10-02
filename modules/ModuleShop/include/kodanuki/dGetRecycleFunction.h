#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

namespace kodanuki {
// vtable +0x29F28 in ModuleShop.cro, offset_to_top 0, 1 entries
class GetRecycleFunction : public ::BsNpcMgr::NpcForeachFunction
{
public:
    GetRecycleFunction(); // ctor address unknown
};
} // namespace kodanuki
