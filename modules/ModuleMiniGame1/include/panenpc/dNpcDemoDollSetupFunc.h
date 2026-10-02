#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

namespace panenpc {
// vtable +0x315DC in ModuleMiniGame1.cro, offset_to_top 0, 1 entries
class NpcDemoDollSetupFunc : public ::BsNpcMgr::NpcForeachFunction
{
public:
    NpcDemoDollSetupFunc(); // ctor address unknown
};
} // namespace panenpc
