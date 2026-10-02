#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_MatchmakeSessionSearchCriteria.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex30MatchmakeSessionSearchCriteriaE @ 0x008CF1C4
// vtable 0x008FF410 (vptr 0x008FF418), offset_to_top 0, 2 entries
class MatchmakeSessionSearchCriteria : public ::nn::nex::_DDL_MatchmakeSessionSearchCriteria
{
public:
    virtual void vf_0x00(); // 0x003C3034 slot 0x00 | fefates:callseq
    virtual void vf_0x04(); // 0x003C2FC0 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_MatchmakeSessionSearchCriteria
    void SetMaxParticipants(unsigned short); // 0x003C2B90 | fefates:bytes [tier B]
    void SetMinParticipants(unsigned short); // 0x003C2BB0 | fefates:bytes [tier B]
    void Reset(); // 0x003C2BCC | fefates:bytes [tier B]
    MatchmakeSessionSearchCriteria(); // 0x003C2F18 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
