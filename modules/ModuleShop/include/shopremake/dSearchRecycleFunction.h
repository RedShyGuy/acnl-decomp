#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

namespace shopremake {
// vtable +0x281A8 in ModuleShop.cro, offset_to_top 0, 1 entries
class SearchRecycleFunction : public ::BsNpcMgr::NpcForeachFunction
{
public:
    SearchRecycleFunction(); // ctor address unknown
};
} // namespace shopremake
