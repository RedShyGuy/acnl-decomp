#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

namespace cafetalk2 {
// vtable +0xDFEC in ModuleCafe.cro, offset_to_top 0, 1 entries
class GetMasterFunction : public ::BsNpcMgr::NpcForeachFunction
{
public:
    GetMasterFunction(); // ctor address unknown
};
} // namespace cafetalk2
