#pragma once

#include "decomp.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_Gathering.h"
#include "nn/nex/nex_MatchmakeParam.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
// The DDL part of MatchmakeSession (MatchmakeExtension): a Gathering with the matchmake data and
// three revisions (1: the MatchmakeParam and the start time, 2: the user password, 3: the refer
// gathering and the password flags). The binary has no DDL declaration of it; the member names
// are ours (from pia::inet::NexMatchmakeSession).
// RTTI N2nn3nex21_DDL_MatchmakeSessionE @ 0x008CEB80
// vtable 0x008FDFC8 (vptr 0x008FDFD0), offset_to_top 0, 9 entries
class _DDL_MatchmakeSession : public ::nn::nex::Gathering
{
public:
    // the highest revision of the structure
    static const u8 REVISION = 3;

    // (inline)
    _DDL_MatchmakeSession() : m_StructureVersion(REVISION), m_Attributes(0), m_ApplicationBuffer(0), m_SessionKey(0) {}
    virtual ~_DDL_MatchmakeSession(); // 0x0039BC30 slot 0x00 | fefates:bytes
    // 0x0039BC20 slot 0x04 (deleting dtor)
    virtual nn::nex::Gathering* Clone() const; // 0x0072D008 slot 0x08 | fefates:callseq
    virtual nn::nex::String GetGatheringType() const; // 0x0072CFA8 slot 0x0C | mk7dlp:bytes
    virtual bool IsA(const nn::nex::String& typeName) const; // 0x0072CFD4 slot 0x10
    virtual bool IsAKindOf(const nn::nex::String& typeName) const; // 0x0072D028 slot 0x14
    virtual void StreamIn(nn::nex::Message* pMessage) const; // 0x0039B71C slot 0x18
    virtual void StreamOut(nn::nex::Message* pMessage); // 0x0039B9B8 slot 0x1C

    static void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_MatchmakeSession& data); // 0x0039B72C | fefates:bytes [tier B]
    static void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_MatchmakeSession* pData); // 0x0039B9C8 | fefates:bytes [tier B]
    // (the copy of the members; name is ours)
    nn::nex::_DDL_MatchmakeSession& operator=(const nn::nex::_DDL_MatchmakeSession& other); // 0x0039BC74
    // (inline; armlink placed it at the end of the code; name is ours)
    u32 GetAttribute(u32 index) const; // 0x0072B750

    u8 m_StructureVersion;              // 0x30
    u32 m_GameMode;                     // 0x34
    qVector<u32> m_Attributes;          // 0x38
    bool m_IsOpenParticipation;         // 0x44
    u32 m_MatchmakeSystemType;          // 0x48
    qVector<u8> m_ApplicationBuffer;    // 0x4C
    u32 m_ParticipationCount;           // 0x58
    u8 m_ProgressScore;                 // 0x5C
    qVector<u8> m_SessionKey;           // 0x60
    u32 m_Unknown0x6C;                  // 0x6C
    // revision 1
    MatchmakeParam m_MatchmakeParam;    // 0x70
    DateTime m_StartedTime;             // 0x98
    // revision 2
    String m_UserPassword;              // 0xA0
    // revision 3
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
