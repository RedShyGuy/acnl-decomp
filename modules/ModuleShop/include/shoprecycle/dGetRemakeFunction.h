#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

namespace shoprecycle {
// vtable +0x281C0 in ModuleShop.cro, offset_to_top 0, 1 entries
class GetRemakeFunction : public ::BsNpcMgr::NpcForeachFunction
{
public:
    GetRemakeFunction(); // ctor address unknown
};
} // namespace shoprecycle
