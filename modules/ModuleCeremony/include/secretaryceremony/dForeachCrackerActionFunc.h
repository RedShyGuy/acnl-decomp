#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

namespace secretaryceremony {
// vtable +0x586C in ModuleCeremony.cro, offset_to_top 0, 1 entries
class ForeachCrackerActionFunc : public ::BsNpcMgr::NpcForeachFunction
{
public:
    ForeachCrackerActionFunc(); // ctor address unknown
};
} // namespace secretaryceremony
