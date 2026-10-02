#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

namespace panenpc {
// vtable +0x315D0 in ModuleMiniGame1.cro, offset_to_top 0, 1 entries
class NpcDeleteRequestFunc : public ::BsNpcMgr::NpcForeachFunction
{
public:
    NpcDeleteRequestFunc(); // ctor address unknown
};
} // namespace panenpc
