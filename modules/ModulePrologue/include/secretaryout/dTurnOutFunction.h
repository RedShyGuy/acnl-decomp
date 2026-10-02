#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

namespace secretaryout {
// vtable +0x8B80 in ModulePrologue.cro, offset_to_top 0, 1 entries
class TurnOutFunction : public ::BsNpcMgr::NpcForeachFunction
{
public:
    TurnOutFunction(); // ctor address unknown
};
} // namespace secretaryout
