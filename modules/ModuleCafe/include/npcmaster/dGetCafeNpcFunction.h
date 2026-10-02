#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

namespace npcmaster {
// vtable +0xDFF8 in ModuleCafe.cro, offset_to_top 0, 1 entries
class GetCafeNpcFunction : public ::BsNpcMgr::NpcForeachFunction
{
public:
    GetCafeNpcFunction(); // ctor address unknown
};
} // namespace npcmaster
