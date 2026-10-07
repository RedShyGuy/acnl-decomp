#pragma once

#include "decomp.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_Gathering.h"
#include "nn/nex/nex_MatchmakeParam.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex21_DDL_MatchmakeSessionE @ 0x008CEB80
// vtable 0x008FDFC8 (vptr 0x008FDFD0), offset_to_top 0, 9 entries
//
// The members are from pia::inet::NexMatchmakeSession (the names are ours; Clone creates 176
// bytes).
class _DDL_MatchmakeSession : public ::nn::nex::Gathering
{
public:
    _DDL_MatchmakeSession(); // ctor address unknown
    virtual ~_DDL_MatchmakeSession(); // 0x0039BC30 slot 0x00 | fefates:bytes
    // 0x0039BC20 slot 0x04 | slot vf_0x04 of nn::nex::_DDL_Gathering (deleting dtor)
    virtual void Clone() const; // 0x0072D008 slot 0x08 | fefates:callseq
    virtual void GetGatheringType() const; // 0x0072CFA8 slot 0x0C | mk7dlp:bytes
    virtual bool IsA(const String& typeName) const; // 0x0072CFD4 slot 0x10
    virtual bool IsAKindOf(const String& typeName) const; // 0x0072D028 slot 0x14
    virtual void StreamIn(nn::nex::Message*) const; // 0x0039B71C slot 0x18 | slot vf_0x18 of nn::nex::_DDL_Gathering
    virtual void StreamOut(nn::nex::Message*); // 0x0039B9B8 slot 0x1C | slot vf_0x1C of nn::nex::_DDL_Gathering
    void Extract(nn::nex::Message*, nn::nex::_DDL_MatchmakeSession*); // 0x0039B9C8 | fefates:bytes [tier B]
    // (the copy of the members; name is ours)
    void operator=(const _DDL_MatchmakeSession& other); // 0x0039BC74

    // (inline; armlink placed it at the end of the code; name is ours)
    u32 GetAttribute(u32 index) const; // 0x0072B750

    u8 m_Unknown0x30;                   // 0x30
    u32 m_GameMode;                     // 0x34
    qVector<u32> m_Attributes;          // 0x38
    bool m_IsOpenParticipation;         // 0x44
    u32 m_MatchmakeSystemType;          // 0x48
    qVector<u8> m_ApplicationBuffer;    // 0x4C
    u32 m_ParticipationCount;           // 0x58
    u8 m_ProgressScore;                 // 0x5C
    qVector<u8> m_SessionKey;           // 0x60
    u32 m_Unknown0x6C;                  // 0x6C
    MatchmakeParam m_MatchmakeParam;    // 0x70
    u32 m_Unknown0x94;                  // 0x94
    DateTime m_StartedTime;             // 0x98
    String m_UserPassword;              // 0xA0
    u32 m_ReferGatheringId;             // 0xA8
    bool m_IsUserPasswordEnabled;       // 0xAC
    bool m_IsSystemPasswordEnabled;     // 0xAD
};
ASSERT_OFFSET(_DDL_MatchmakeSession, m_Attributes, 0x38);
ASSERT_OFFSET(_DDL_MatchmakeSession, m_ApplicationBuffer, 0x4C);
ASSERT_OFFSET(_DDL_MatchmakeSession, m_MatchmakeParam, 0x70);
ASSERT_OFFSET(_DDL_MatchmakeSession, m_StartedTime, 0x98);
ASSERT_OFFSET(_DDL_MatchmakeSession, m_IsSystemPasswordEnabled, 0xAD);
ASSERT_SIZE(_DDL_MatchmakeSession, 0xB0);
} // namespace nex
} // namespace nn
