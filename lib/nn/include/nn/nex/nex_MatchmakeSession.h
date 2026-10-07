#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_MatchmakeSession.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex16MatchmakeSessionE @ 0x008CE5CC
// vtable 0x008FD1B4 (vptr 0x008FD1BC), offset_to_top 0, 9 entries
class MatchmakeSession : public ::nn::nex::_DDL_MatchmakeSession
{
public:
    virtual ~MatchmakeSession(); // 0x00382704 slot 0x00 | slot vf_0x00 of nn::nex::_DDL_Gathering
    // 0x003826F4 slot 0x04 | slot vf_0x04 of nn::nex::_DDL_Gathering (deleting dtor)
    void Reset(); // 0x0038254C | fefates:bytes [tier B]
    MatchmakeSession(); // 0x0038262C | fefates:bytes [tier B]
    void SetMatchmakeSystemType(nn::nex::MatchmakeSystemType, unsigned int); // 0x003D6A1C | fefates:bytes [tier B]
    // (names are ours)
    void SetAttribute(u32 index, u32 value); // 0x0038251C
    // false if the score is above 100
    bool SetProgressScore(u8 score); // 0x00382528
    void SetApplicationBuffer(const qVector<u8>& buffer); // 0x0038253C
};
} // namespace nex
} // namespace nn
