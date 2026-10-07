#pragma once

#include "decomp.h"
#include "nn/nex/nex_MatchmakeParam.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qVector.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex35_DDL_MatchmakeSessionSearchCriteriaE @ 0x008CF384
// vtable 0x008FF724 (vptr 0x008FF72C), offset_to_top 0, 2 entries
class _DDL_MatchmakeSessionSearchCriteria : public ::nn::nex::RootObject
{
public:
    _DDL_MatchmakeSessionSearchCriteria(); // ctor address unknown
    virtual void vf_0x00(); // 0x003CBBCC slot 0x00 | fefates:callseq
    virtual void vf_0x04(); // 0x003CBB58 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_MatchmakeSessionSearchCriteria
    void Add(nn::nex::Message*, const nn::nex::_DDL_MatchmakeSessionSearchCriteria&); // 0x003CB984 | fefates:bytes [tier B]

    // (the layout is from MatchmakeSessionSearchCriteria::MatchmakeSessionSearchCriteria and its
    // setters; the names are ours)
    u8 m_Unknown0x4;                    // 0x04
    qVector<String> m_Attributes;       // 0x08, a string per attribute ("" or "%u" or "%u,%u")
    String m_GameMode;                  // 0x14
    String m_MinParticipants;           // 0x1C
    String m_MaxParticipants;           // 0x24
    String m_MatchmakeSystemType;       // 0x2C
    bool m_IsVacantOnly;                // 0x34
    bool m_Unknown0x35;                 // 0x35
    bool m_Unknown0x36;                 // 0x36
    u32 m_Unknown0x38;                  // 0x38
    u16 m_Unknown0x3C;                  // 0x3C, 1 with m_IsVacantOnly
    MatchmakeParam m_MatchmakeParam;    // 0x40
    bool m_Unknown0x64;                 // 0x64
    u32 m_Unknown0x68;                  // 0x68
};
ASSERT_OFFSET(_DDL_MatchmakeSessionSearchCriteria, m_GameMode, 0x14);
ASSERT_OFFSET(_DDL_MatchmakeSessionSearchCriteria, m_MatchmakeParam, 0x40);
ASSERT_SIZE(_DDL_MatchmakeSessionSearchCriteria, 0x6C);
} // namespace nex
} // namespace nn
