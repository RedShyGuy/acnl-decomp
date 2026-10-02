#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

// vtable +0x8718 in ModuleNpcDt.cro, offset_to_top 0, 1 entries
class SearchMasterCB : public ::BsNpcMgr::NpcForeachFunction
{
public:
    SearchMasterCB(); // ctor address unknown
};
