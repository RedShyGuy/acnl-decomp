#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_SignatureSettingWithKeyBuffer.h"
#include "nn/pia/session/session_IMatchmakeSession.h"
#include "nn/pia/session/session_ISessionInfoList.h"

namespace nn {
namespace pia {
namespace transport {
class StationConnectionInfo;
}
namespace session {
// RTTI N2nn3pia7session22CommonMatchmakeSessionE @ 0x008D00C8
// vtable 0x00901AE4 (vptr 0x00901AEC), offset_to_top 0, 41 entries
//
// The base of the matchmake sessions of the networks (inet::NexMatchmakeSession,
// local::LocalMatchmakeSession): the signature setting of the session and a few values; most slots
// are pure. Layout from the constructor; the member names and the slot names are ours.
class CommonMatchmakeSession : public ::nn::pia::session::IMatchmakeSession
{
public:
    CommonMatchmakeSession(); // 0x00440FC4
    virtual ~CommonMatchmakeSession(); // 0x0044102C slot 0x00
    // 0x00441028 slot 0x04 (deleting dtor)
    // clears the signature key
    virtual void Cleanup(); // 0x00440F9C slot 0x08
    // the automatic matchmaking (AutoMatchmakeJob; names are ours): the session, whether the
    // local station created it, the joint session it belongs to and its application data
    virtual nn::Result AutoMatchmakeAsync(nn::pia::common::CallContext* pCallContext) = 0; // slot 0x0C
    virtual bool IsAutoMatchmakeCompleted(u32* pSessionId, bool* pIsCreator, u32* pJointSessionId, void* pApplicationData, u32* pApplicationDataSize) = 0; // slot 0x10
    // the search of sessions (BrowseMatchmakeJob; names are ours)
    virtual nn::Result BrowseAsync(nn::pia::common::CallContext* pCallContext) = 0; // slot 0x14
    virtual bool IsBrowseCompleted() = 0; // slot 0x18
    // the list the searches fill (Session's)
    virtual nn::pia::session::ISessionInfoList* GetSessionInfoList() = 0; // slot 0x1C
    // the creation of the session (CreateSessionJob; names are ours): the id of the new session
    virtual nn::Result CreateAsync(nn::pia::common::CallContext* pCallContext) = 0; // slot 0x20
    virtual bool IsCreateCompleted(u32* pSessionId) = 0; // slot 0x24
    // the join of a session (JoinSessionJob; names are ours): the joint session the session
    // belongs to and its application data
    virtual nn::Result JoinAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId) = 0; // slot 0x28
    virtual bool IsJoinCompleted(u32* pJointSessionId, void* pApplicationData, u32* pApplicationDataSize) = 0; // slot 0x2C
    // leaves the session on the server (inet: the gathering)
    virtual nn::Result LeaveAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId) = 0; // slot 0x30
    virtual bool IsLeaveCompleted() = 0; // slot 0x34
    // unregisters the session from the server (inet: the gathering)
    virtual nn::Result UnregisterAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId) = 0; // slot 0x38
    virtual bool IsUnregisterCompleted() = 0; // slot 0x3C
    // the participation of other stations (OpenParticipationJob / CloseParticipationJob; names
    // are ours)
    virtual nn::Result OpenParticipationAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId) = 0; // slot 0x40
    virtual bool IsOpenParticipationCompleted() = 0; // slot 0x44
    virtual nn::Result CloseParticipationAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId) = 0; // slot 0x48
    virtual bool IsCloseParticipationCompleted() = 0; // slot 0x4C
    // the connection info of the host of the session (AutoMatchmakeJob): start, and whether it
    // is done (then the info is filled)
    virtual nn::Result vf_0x50(nn::pia::common::CallContext* pCallContext, u32 sessionId) = 0; // slot 0x50
    virtual bool vf_0x54(nn::pia::transport::StationConnectionInfo* pInfo) = 0; // slot 0x54
    // an attribute of the session is changed (ModifyAttributeJob; names are ours)
    virtual nn::Result ModifyAttributeAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId, u32 index, u32 value) = 0; // slot 0x58
    virtual bool IsModifyAttributeCompleted() = 0; // slot 0x5C
    // inet: the gathering is looked up, then the value of an attribute of it is read
    virtual nn::Result vf_0x60(nn::pia::common::CallContext* pCallContext, u32 sessionId) = 0; // slot 0x60
    virtual bool vf_0x64(u32 index, u32* pValue) = 0; // slot 0x64
    virtual u16 vf_0x68() const; // 0x00734078 slot 0x68
    // the change of the session setting (UpdateSessionSettingJob; names are ours)
    virtual nn::Result UpdateSessionSettingAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x00440EF8 slot 0x6C
    virtual bool IsUpdateSessionSettingCompleted(); // 0x00440F18 slot 0x70
    virtual nn::Result vf_0x74(u32 value) = 0; // slot 0x74
    // the check of the session status (SessionStatusCheckJob): start and whether it is done
    virtual nn::Result vf_0x78(nn::pia::common::CallContext* pCallContext, u32 sessionId) = 0; // slot 0x78
    virtual bool vf_0x7C() = 0; // slot 0x7C
    virtual nn::pia::common::SignatureSetting* GetSignatureSetting(); // 0x00440F30 slot 0x80
    // the key (only HMAC keys of KEY_SIZE bytes) goes into the own buffer
    virtual nn::Result SetSignatureSetting(nn::pia::common::SignatureSetting::Mode mode, const void* pKey, u32 keySize); // 0x00440F38 slot 0x84
    virtual bool vf_0x88() const; // 0x00734068 slot 0x88
    virtual void vf_0x8C(bool value); // 0x00440EF0 slot 0x8C
    virtual u32 vf_0x90() const; // 0x00734070 slot 0x90
    virtual void vf_0x94(u32 value); // 0x00440F20 slot 0x94
    // the progress score of the session (0 to 100) on the server; inet: at most every 30
    // seconds, except for 0 and 100 (names are ours)
    virtual nn::Result UpdateProgressScore(u32 sessionId, u8 score); // 0x00440F0C slot 0x98
    virtual bool IsProgressScoreUpdatable(u8 score); // 0x00440F28 slot 0x9C
    virtual bool vf_0xA0() const; // 0x00440F04 slot 0xA0

    static const u32 KEY_SIZE = 32;

    bool m_Unknown0x4;                                              // 0x04
    u16 m_Unknown0x6;                                               // 0x06
    common::SignatureSettingWithKeyBuffer<KEY_SIZE> m_SignatureSetting; // 0x08
    u32 m_Unknown0x38;                                              // 0x38
    bool m_Unknown0x3C;                                             // 0x3C
};
ASSERT_OFFSET(CommonMatchmakeSession, m_SignatureSetting, 0x8);
ASSERT_OFFSET(CommonMatchmakeSession, m_Unknown0x38, 0x38);
ASSERT_SIZE(CommonMatchmakeSession, 0x40);
} // namespace session
} // namespace pia
} // namespace nn
