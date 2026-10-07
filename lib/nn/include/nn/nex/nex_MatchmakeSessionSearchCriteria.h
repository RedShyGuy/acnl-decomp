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
    // fefates has the names at +4 with one value; this version formats two values into the
    // range string (its first instruction moves the second one)
    void SetMaxParticipants(u16 value1, u16 value2); // 0x003C2B8C | fefates:bytes [tier B]
    void SetMinParticipants(u16 value1, u16 value2); // 0x003C2BAC | fefates:bytes [tier B]
    void Reset(); // 0x003C2BCC | fefates:bytes [tier B]
    // (names are ours)
    void SetGameMode(u32 gameMode); // 0x003C2A58
    // the values of the attribute (at most 100); false if there are none or too many
    bool SetAttribute(u32 index, const qVector<u32>& values); // 0x003C2A6C
    void SetAttribute(u32 index, u32 value); // 0x003C2B34
    void SetVacantOnly(bool isVacantOnly); // 0x003C2B4C
    void SetAttributeRange(u32 index, u32 min, u32 max); // 0x003C2B5C
    void SetMatchmakeSystemType(u32 type); // 0x003C2B78
    // m_Unknown0x38 and the parameters
    void SetMatchmakeParam(u32 value, const MatchmakeParam& param); // 0x00395238
    MatchmakeSessionSearchCriteria(); // 0x003C2F18 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
