#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalSessionSearchCriteria.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"

namespace nn {
namespace pia {
namespace local {
class LocalCreateSessionSetting;
class LocalNetwork;
class LocalSessionInfo;
struct LocalConnectNetworkSetting;
struct LocalCreateNetworkSetting;

// RTTI N2nn3pia5local21LocalMatchmakeSessionE @ 0x008CFC04
// vtable 0x00900E40 (vptr 0x00900E48), offset_to_top 0, 45 entries
//
// The matchmake session of the local network: a session is a network of LocalNetwork. Create,
// join, leave, unregister (destroy) and browse (scan) start the asynchronous calls of the
// network; the Is*Completed slots signal the call context. There is no server, so attributes,
// the automatic matchmaking and the host info are not supported. The layout is from the
// constructor; the member names and the names of the new slots are ours.
class LocalMatchmakeSession : public ::nn::pia::session::CommonMatchmakeSession
{
public:
    // the running call of the network
    enum Request : u8
    {
        REQUEST_NONE = 0,
        REQUEST_CREATE = 1,
        REQUEST_DESTROY = 2,
        REQUEST_SCAN = 3,
        REQUEST_CONNECT = 4,
        REQUEST_DISCONNECT = 5,
    };

    LocalMatchmakeSession(); // 0x0041CB38
    virtual ~LocalMatchmakeSession(); // 0x0041CBCC slot 0x00
    // 0x0041CBB4 slot 0x04 (deleting dtor)
    virtual void Cleanup(); // 0x0041CAA8 slot 0x08
    virtual nn::Result AutoMatchmakeAsync(nn::pia::common::CallContext* pCallContext); // 0x0041C518 slot 0x0C
    virtual bool IsAutoMatchmakeCompleted(u32* pSessionId, bool* pIsCreator, u32* pJointSessionId, void* pApplicationData, u32* pApplicationDataSize); // 0x0041C7E0 slot 0x10
    virtual nn::Result BrowseAsync(nn::pia::common::CallContext* pCallContext); // 0x0041C64C slot 0x14
    virtual nn::Result CreateAsync(nn::pia::common::CallContext* pCallContext); // 0x0041C6DC slot 0x20
    virtual nn::Result JoinAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x0041C524 slot 0x28
    virtual bool IsJoinCompleted(u32* pJointSessionId, void* pApplicationData, u32* pApplicationDataSize); // 0x0041C7E8 slot 0x2C
    virtual nn::Result LeaveAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x0041C5C4 slot 0x30
    virtual bool IsLeaveCompleted(); // 0x0041C8B8 slot 0x34
    virtual nn::Result UnregisterAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x0041C960 slot 0x38
    virtual bool IsUnregisterCompleted(); // 0x0041CA24 slot 0x3C
    virtual nn::Result OpenParticipationAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x0041C3C8 slot 0x40
    virtual bool IsOpenParticipationCompleted(); // 0x0041C510 slot 0x44
    virtual nn::Result CloseParticipationAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x0041C464 slot 0x48
    virtual bool IsCloseParticipationCompleted(); // 0x0041C5BC slot 0x4C
    virtual nn::Result vf_0x50(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x0041C7D4 slot 0x50
    virtual bool vf_0x54(nn::pia::transport::StationConnectionInfo* pInfo); // 0x0041CA1C slot 0x54
    virtual nn::Result ModifyAttributeAsync(nn::pia::common::CallContext* pCallContext, u32 sessionId, u32 index, u32 value); // 0x0041C3BC slot 0x58
    virtual bool IsModifyAttributeCompleted(); // 0x0041C508 slot 0x5C
    virtual nn::Result vf_0x60(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x0041C3B0 slot 0x60
    virtual bool vf_0x64(u32 index, u32* pValue); // 0x0041C500 slot 0x64
    virtual u16 vf_0x68() const; // 0x00731230 slot 0x68
    virtual nn::Result vf_0x74(u32 value); // 0x0041C9E8 slot 0x74
    virtual nn::Result vf_0x78(nn::pia::common::CallContext* pCallContext, u32 sessionId); // 0x0041C7B0 slot 0x78
    // the participation is open (LocalNetwork::GetParticipationState)
    virtual bool vf_0x7C(); // 0x0041C9F0 slot 0x7C
    // the local station is the host of the network
    virtual bool vf_0x88() const; // 0x007311E8 slot 0x88
    // the transport id of the host of the network (255 without a network)
    virtual u32 vf_0x90() const; // 0x0073120C slot 0x90
    virtual nn::Result vf_0xA4(); // 0x0041CB2C slot 0xA4
    // the network setting of the session setting (CreateSessionJob)
    virtual void SetCreateSessionSetting(const nn::pia::local::LocalCreateSessionSetting* pSetting); // 0x0041CAA4 slot 0xA8
    // the network of the found session and its passphrase (JoinSessionJob)
    virtual nn::Result SetJoinSetting(const nn::pia::local::LocalSessionInfo* pInfo, const void* pPassphrase, u32 passphraseSize) = 0; // slot 0xAC
    virtual nn::pia::local::LocalConnectNetworkSetting* GetConnectNetworkSetting() = 0; // slot 0xB0

    // (names are ours)
    void SetSearchCriteria(const nn::pia::local::LocalSessionSearchCriteria* pCriteria); // 0x0041C764
    nn::Result SetApplicationData(const void* pData, u32 size); // 0x0041C938

    nn::pia::local::LocalNetwork* m_pNetwork;                        // 0x40
    nn::pia::local::LocalCreateNetworkSetting* m_pCreateNetworkSetting; // 0x44
    nn::pia::local::LocalConnectNetworkSetting* m_pConnectNetworkSetting; // 0x48
    nn::pia::local::LocalSessionSearchCriteria m_SearchCriteria;     // 0x4C
    Request m_Request;                                               // 0x6C
    nn::pia::common::CallContext* m_pCallContext;                    // 0x70
};
ASSERT_OFFSET(LocalMatchmakeSession, m_pNetwork, 0x40);
ASSERT_OFFSET(LocalMatchmakeSession, m_SearchCriteria, 0x4C);
ASSERT_SIZE(LocalMatchmakeSession, 0x74);
} // namespace local
} // namespace pia
} // namespace nn
