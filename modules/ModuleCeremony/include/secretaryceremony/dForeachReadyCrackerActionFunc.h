#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

namespace secretaryceremony {
// vtable +0x5884 in ModuleCeremony.cro, offset_to_top 0, 1 entries
class ForeachReadyCrackerActionFunc : public ::BsNpcMgr::NpcForeachFunction
{
public:
    ForeachReadyCrackerActionFunc(); // ctor address unknown
};
} // namespace secretaryceremony
