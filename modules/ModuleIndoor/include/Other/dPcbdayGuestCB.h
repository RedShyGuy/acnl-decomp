#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

// vtable +0x65E00 in ModuleIndoor.cro, offset_to_top 0, 1 entries
class PcbdayGuestCB : public ::BsNpcMgr::NpcForeachFunction
{
public:
    PcbdayGuestCB(); // ctor address unknown
};
