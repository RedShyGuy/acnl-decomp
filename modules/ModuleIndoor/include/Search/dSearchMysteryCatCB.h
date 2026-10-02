#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

// vtable +0x66BF0 in ModuleIndoor.cro, offset_to_top 0, 1 entries
class SearchMysteryCatCB : public ::BsNpcMgr::NpcForeachFunction
{
public:
    SearchMysteryCatCB(); // ctor address unknown
};
