#pragma once

#include "decomp.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_MatchmakeParam.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex32_DDL_UpdateMatchmakeSessionParamE @ 0x008CF2EC
// vtable 0x008FF698 (vptr 0x008FF6A0), offset_to_top 0, 2 entries
class _DDL_UpdateMatchmakeSessionParam : public ::nn::nex::RootObject
{
public:
    _DDL_UpdateMatchmakeSessionParam(); // ctor address unknown
    virtual void vf_0x00(); // 0x003CB0B8 slot 0x00 | virtual slot, introduced by nn::nex::_DDL_UpdateMatchmakeSessionParam
    virtual void vf_0x04(); // 0x003CB074 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_UpdateMatchmakeSessionParam

    // (the layout is from pia::inet::NexMatchmakeSession, which inlines the constructor; the
    // names are ours)
    u8 m_Unknown0x4;                     // 0x04
    u32 m_GatheringId;                   // 0x08
    u32 m_ModificationFlags;             // 0x0C
    qVector<u32> m_Attributes;           // 0x10
    bool m_IsOpenParticipation;          // 0x1C
    qVector<u8> m_ApplicationBuffer;     // 0x20
    u8 m_ProgressScore;                  // 0x2C
    MatchmakeParam m_MatchmakeParam;     // 0x30
    DateTime m_StartedTime;              // 0x58
    String m_UserPassword;               // 0x60
    u32 m_GameMode;                      // 0x68
    String m_Description;                // 0x6C
    u16 m_MinParticipants;               // 0x74
    u16 m_MaxParticipants;               // 0x76
    u32 m_MatchmakeSystemType;           // 0x78
    u32 m_ParticipationPolicy;           // 0x7C
    u32 m_PolicyArgument;                // 0x80
    u32 m_Unknown0x84;                   // 0x84
};
ASSERT_OFFSET(_DDL_UpdateMatchmakeSessionParam, m_ApplicationBuffer, 0x20);
ASSERT_OFFSET(_DDL_UpdateMatchmakeSessionParam, m_MatchmakeParam, 0x30);
ASSERT_OFFSET(_DDL_UpdateMatchmakeSessionParam, m_Description, 0x6C);
ASSERT_SIZE(_DDL_UpdateMatchmakeSessionParam, 0x88);
} // namespace nex
} // namespace nn
