#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

// vtable +0xF730 in ModuleTrain.cro, offset_to_top 0, 1 entries
class TrainSearchPartnerFunction : public ::BsNpcMgr::NpcForeachFunction
{
public:
    TrainSearchPartnerFunction(); // ctor address unknown
};
