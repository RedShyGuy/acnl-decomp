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
    // the values of a reset and the participation policies of the matchmake system types (the
    // names are ours)
    static const u32 DEFAULT_ATTRIBUTE_COUNT = 6;
    static const u8 DEFAULT_PROGRESS_SCORE = 100;
    static const u32 PARTICIPATION_POLICY_DEFAULT = 96;
    static const u32 PARTICIPATION_POLICY_SYSTEM_TYPE_1 = 95;
    static const u32 PARTICIPATION_POLICY_SYSTEM_TYPE_2_3 = 98;

    MatchmakeSession(); // 0x0038262C | fefates:bytes [tier B]
    virtual ~MatchmakeSession(); // 0x00382704 slot 0x00
    // 0x003826F4 slot 0x04 (deleting dtor)
    void Reset(); // 0x0038254C | fefates:bytes [tier B]
    // the participation policy follows the type (the argument for the types 1-3)
    void SetMatchmakeSystemType(nn::nex::MatchmakeSystemType type, unsigned int argument); // 0x003D6A1C | fefates:bytes [tier B]
    // (names are ours)
    void SetAttribute(u32 index, u32 value); // 0x0038251C
    // false if the score is above 100
    bool SetProgressScore(u8 score); // 0x00382528
    void SetApplicationBuffer(const qVector<u8>& buffer); // 0x0038253C
};
ASSERT_SIZE(MatchmakeSession, 0xB0);
} // namespace nex
} // namespace nn
