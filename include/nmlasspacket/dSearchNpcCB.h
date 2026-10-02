#pragma once

#include "decomp.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"

namespace nmlasspacket {
// RTTI N12nmlasspacket11SearchNpcCBE @ 0x008CD920
// vtable 0x008FB1C4 (vptr 0x008FB1CC), offset_to_top 0, 1 entries
class SearchNpcCB : public ::BsNpcMgr::NpcForeachFunction
{
public:
    SearchNpcCB(); // ctor candidate(s) 0x0034306C, 0x00343A80, 0x004DB830, 0x004DB94C, 0x004DBAE8 (unverified)
    virtual void vf_0x00(); // 0x0020E720 slot 0x00 | virtual slot, introduced by nmlasspacket::SearchNpcCB
};
} // namespace nmlasspacket
