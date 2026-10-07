#pragma once

#include "decomp.h"
#include "nn/pia/session/session_SessionSearchCriteria.h"

namespace nn {
namespace nex {
class MatchmakeParam;
class MatchmakeSessionSearchCriteria;
class ResultRange;
} // namespace nex
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet24NexSessionSearchCriteriaE @ 0x008CF9F0
// vtable 0x0090059C (vptr 0x009005A4), offset_to_top 0, 3 entries
//
// The search criteria of the inet network: the setters note the values and a flag each,
// ConvertTo fills the nex search criteria (and the monitoring data) with the set ones. The layout
// is from the constructor and ConvertTo; the member names and the names of the setters without
// symbol are ours.
class NexSessionSearchCriteria : public ::nn::pia::session::SessionSearchCriteria
{
public:
    static const u32 ATTRIBUTE_NUM = 6;
    static const u32 ATTRIBUTE_VALUE_NUM_MAX = 100;
    static const u32 RESULT_NUM_MAX = 100;

    // what was set (m_Flags; names are ours)
    enum Flag
    {
        FLAG_MIN_PARTICIPANTS = 1 << 0,
        FLAG_MAX_PARTICIPANTS = 1 << 1,
        FLAG_OPENED_ONLY = 1 << 2,
        FLAG_VACANT_ONLY = 1 << 3,
        FLAG_GAME_MODE = 1 << 4,
        FLAG_MATCHMAKE_SYSTEM_TYPE = 1 << 5,
        FLAG_ATTRIBUTE = 1 << 6,
        FLAG_UNKNOWN_0x9D2 = 1 << 7,
        FLAG_UNKNOWN_0x9D3 = 1 << 8,
        FLAG_PARAM_SI = 1 << 9,
        FLAG_PARAM_RV = 1 << 10,
        FLAG_PARAM_DR = 1 << 11,
        FLAG_PARAM_VR = 1 << 12,
        FLAG_PARAM_NCC = 1 << 13,
        FLAG_PARAM_USGI = 1 << 14,
        FLAG_PARAM_OIA = 1 << 15,
        FLAG_UNKNOWN_0xA10 = 1 << 16,
    };

    NexSessionSearchCriteria(); // 0x0040BC10 | libgarden [tier A]
    virtual ~NexSessionSearchCriteria(); // 0x0040BCF8 slot 0x00
    // 0x0040BCF0 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072F84C slot 0x08

    void SetGameMode(unsigned long gameMode); // 0x0040B298 | libgarden [tier A]
    void SetAttribute(u32 index, u32 value); // 0x0040B2B4 (name is ours)
    void SetOpenedOnly(bool isOpenedOnly); // 0x0040B304 | libgarden [tier A]
    void SetVacantOnly(bool isVacantOnly); // 0x0040B320 (name is ours)
    void SetMatchmakeSystemType(u8 type); // 0x0040B33C (name is ours)
    void SetMaxParticipants(unsigned short num); // 0x0040B358 | libgarden [tier A]
    void SetMinParticipants(unsigned short num); // 0x0040B378 | libgarden [tier A]
    // the nex search criteria, its parameters and the range of the results; index is the block of
    // the monitoring data (0 or 1, others none; name is ours)
    void ConvertTo(nex::MatchmakeSessionSearchCriteria* pCriteria, nex::MatchmakeParam* pParam, nex::ResultRange* pRange,
                   u32 index) const; // 0x0040B398

    u16 m_MinParticipants[2];                                         // 0x010, 0xFFFF: not set
    u16 m_MaxParticipants[2];                                         // 0x014
    bool m_IsOpenedOnly;                                              // 0x018
    bool m_IsVacantOnly;                                              // 0x019
    u32 m_GameMode;                                                   // 0x01C
    u8 m_MatchmakeSystemType;                                         // 0x020
    u32 m_AttributeValues[ATTRIBUTE_NUM][ATTRIBUTE_VALUE_NUM_MAX];    // 0x024
    u32 m_AttributeValueNum[ATTRIBUTE_NUM];                           // 0x984
    u32 m_AttributeMin[ATTRIBUTE_NUM];                                // 0x99C
    u32 m_AttributeMax[ATTRIBUTE_NUM];                                // 0x9B4
    bool m_IsAttributeRange[ATTRIBUTE_NUM];                           // 0x9CC
    bool m_Unknown0x9D2;                                              // 0x9D2
    bool m_Unknown0x9D3;                                              // 0x9D3
    u8 m_Unknown0x9D4;                                                // 0x9D4, 2: the parameters are sent
    u32 m_ParamSI;                                                    // 0x9D8
    u32 m_ParamRV;                                                    // 0x9DC
    u32 m_ParamDR;                                                    // 0x9E0
    u32 m_ParamVR;                                                    // 0x9E4
    u32 m_ParamNCC;                                                   // 0x9E8
    bool m_ParamUsGI;                                                 // 0x9EC
    wchar_t m_ParamOIA[16];                                           // 0x9EE
    u32 m_Unknown0xA10;                                               // 0xA10
    u32 m_Flags;                                                      // 0xA14, Flag
};
ASSERT_OFFSET(NexSessionSearchCriteria, m_AttributeValues, 0x24);
ASSERT_OFFSET(NexSessionSearchCriteria, m_IsAttributeRange, 0x9CC);
ASSERT_OFFSET(NexSessionSearchCriteria, m_ParamOIA, 0x9EE);
ASSERT_OFFSET(NexSessionSearchCriteria, m_Flags, 0xA14);
ASSERT_SIZE(NexSessionSearchCriteria, 0xA18);
} // namespace inet
} // namespace pia
} // namespace nn
