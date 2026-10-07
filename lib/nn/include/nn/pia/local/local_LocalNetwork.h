#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_CriticalSection.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"

namespace nn {
namespace pia {
namespace common {
class StationAddress;
} // namespace common
namespace local {
class LocalAroundNetworkSearchManager;
class LocalBackgroundProcessJob;
class LocalConnectNetworkJob;
class LocalCreateNetworkJob;
class LocalDestroyNetworkJob;
class LocalDisconnectNetworkJob;
class LocalForceDisconnectNetworkJob;
class LocalMigrationManager;
class LocalNetworkDescription;
class LocalNetworkManager;
class LocalNetworkSetting;
class LocalScanNetworkJob;
class UdsNetworkDescription;
class UdsNetworkSetting;

// RTTI N2nn3pia5local12LocalNetworkE @ 0x008CFAD4
// vtable 0x00900880 (vptr 0x00900888), offset_to_top 0, 3 entries
//
// The local network of pia (a singleton): the scan, creation, connection and end of a network
// with jobs, synchronously (with a CallContext of the caller) or asynchronously (with the own
// CallContext, one request at a time), the managers of the nodes, of the host migration and of the
// search of the networks around. The layout is from the constructor and InitializeCore; the member
// names and the names of the unnamed functions are ours.
class LocalNetwork : public ::nn::pia::common::RootObject
{
public:
    static const u32 NETWORK_DESCRIPTION_NUM = 16;
    static const u32 RECEIVE_BUFFER_SIZE_MIN = 0x4000;
    static const u32 RECEIVE_BUFFER_ALIGNMENT = 0x1000;
    static const u32 SCAN_BUFFER_SIZE_MIN = 0x400;

    // the request of the own CallContext
    enum AsyncType : u8
    {
        ASYNC_TYPE_NONE = 0,
        ASYNC_TYPE_CREATE_NETWORK = 1,
        ASYNC_TYPE_DESTROY_NETWORK = 2,
        ASYNC_TYPE_SCAN_NETWORK = 3,
        ASYNC_TYPE_CONNECT_NETWORK = 4,
        ASYNC_TYPE_DISCONNECT_NETWORK = 5,
    };

    // m_ParticipationState
    enum ParticipationState : u8
    {
        PARTICIPATION_STATE_NONE = 0,
        PARTICIPATION_STATE_ALLOWED = 1,
        PARTICIPATION_STATE_DISALLOWED_WITH_SPECTATORS = 2,
        PARTICIPATION_STATE_DISALLOWED = 3,
    };

    // m_DisconnectReason
    enum DisconnectReason : u8
    {
        DISCONNECT_REASON_NONE = 0,
        DISCONNECT_REASON_DESTROYED_BY_HOST = 4, // LocalDestroyNetworkMessage
    };

    LocalNetwork(); // 0x004164D0 | fefates:bytes [tier B]
    virtual ~LocalNetwork(); // 0x00416600 slot 0x00
    // 0x004165D8 slot 0x04 (deleting dtor)
    virtual void vf_0x08(); // 0x00730034 slot 0x08

    static nn::Result CreateInstance(); // 0x004152EC (name is ours, as LocalFacade's)
    static void DestroyInstance(); // 0x004157F8 | fefates:bytes [tier B]

    nn::Result Initialize(const nn::pia::local::LocalNetworkSetting& setting); // 0x00414B90 | fefates:bytes [tier B]
    void Finalize(); // 0x00416314 | fefates:bytes [tier B]
    nn::Result Startup(); // 0x00416298 | fefates:bytes [tier B]
    void Cleanup(); // 0x00416210 | fefates:bytes-fuzzy [tier B]

    // the requests with a CallContext of the caller
    nn::Result ScanNetwork(nn::pia::common::CallContext* pCallContext, u32 localCommunicationId, u8 subId); // 0x00414EF0 | fefates:bytes-fuzzy [tier B]
    nn::Result CreateNetwork(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalCreateNetworkSetting* pSetting); // 0x0041505C (name is ours)
    nn::Result ConnectNetwork(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalConnectNetworkSetting* pSetting); // 0x004151C8 (name is ours)
    nn::Result DestroyNetwork(nn::pia::common::CallContext* pCallContext); // 0x00415350 | fefates:bytes-fuzzy [tier B]
    nn::Result DisconnectNetwork(nn::pia::common::CallContext* pCallContext); // 0x00415914 (name is ours)

    // the requests with the own CallContext (names are ours, after ScanNetworkAsync)
    nn::Result ScanNetworkAsync(u32 localCommunicationId, u8 subId); // 0x004158AC | fefates:bytes [tier B]
    nn::Result CreateNetworkAsync(const nn::pia::local::LocalCreateNetworkSetting* pSetting); // 0x00415C60
    nn::Result ConnectNetworkAsync(const nn::pia::local::LocalConnectNetworkSetting* pSetting); // 0x00415CC0
    nn::Result DestroyNetworkAsync(); // 0x00415D20 | fefates:bytes [tier B]
    nn::Result DisconnectNetworkAsync(); // 0x00415F98
    nn::Result GetAsyncResult(u8 asyncType) const; // 0x00415E04
    nn::Result GetScanNetworkAsyncResult() const; // 0x00415DFC
    nn::Result GetCreateNetworkAsyncResult() const; // 0x00415FF8
    nn::Result GetConnectNetworkAsyncResult() const; // 0x00416030
    nn::Result GetDestroyNetworkAsyncResult() const; // 0x00416038
    nn::Result GetDisconnectNetworkAsyncResult() const; // 0x00416104
    bool IsScanNetworkAsyncCompleted() const; // 0x00416000
    bool IsCreateNetworkAsyncCompleted() const; // 0x00416040
    bool IsConnectNetworkAsyncCompleted() const; // 0x004160A4
    bool IsDestroyNetworkAsyncCompleted() const; // 0x004160D4
    bool IsDisconnectNetworkAsyncCompleted() const; // 0x00416190
    nn::Result CancelScanNetworkAsync(); // 0x00415F64
    nn::Result CancelConnectNetworkAsync(); // 0x00416070
    bool IsAsyncRunning() const; // 0x0072FF14

    nn::Result AllowParticipating(); // 0x00415BF4 | fefates:bytes-fuzzy [tier B]
    nn::Result DisallowParticipating(bool isSpectatorDisallowed); // 0x00415D80 | fefates:bytes-fuzzy [tier B]
    // the participation state while connected or during the host migration (as the host sent it)
    u8 GetParticipationState() const; // 0x0072FF34 (name is ours)
    nn::Result EjectClient(const nn::pia::common::StationAddress& address); // 0x00414E9C (name is ours)
    nn::Result SetApplicationData(const void* pData, u32 size); // 0x0041610C (name is ours)

    // a copy of a found network / the found network
    nn::Result GetNetworkDescription(nn::pia::local::LocalNetworkDescription* pDescription, u32 index) const; // 0x00415E38 (name is ours)
    nn::pia::local::LocalNetworkDescription* GetNetworkDescription(u32 index); // 0x00415F20 | fefates:bytes [tier B]
    nn::Result GetApplicationData(void* pBuffer, u32* pSize, u32 bufferSize, const nn::pia::local::LocalNetworkDescription* pDescription) const; // 0x0072FD00 | fefates:bytes-fuzzy [tier B]
    nn::Result GetApplicationDataSize(u32* pSize, const nn::pia::local::LocalNetworkDescription* pDescription) const; // 0x0072FEB0 (name is ours)
    // the session id in the beacon of the found network (0 without one)
    static u32 GetSessionId(const nn::pia::local::LocalNetworkDescription* pDescription); // 0x0072FFC8 (name is ours)
    // the stations of a found network / the link level of a found network
    nn::Result GetStationInfoList(nn::pia::local::LocalStationInfo* pInfos, u8 infoNum, u32 index); // 0x0072FD70 (name is ours)
    nn::Result GetLinkLevel(u8* pLevel, u32 index); // 0x0072FC1C (name is ours)
    // the node of a station of the network (symbols.json: GetStationInfoList(LocalStationInfo*,
    // unsigned char, unsigned int) const; the parameters say otherwise)
    nn::Result GetStationInfo(nn::pia::local::LocalStationInfo* pInfo, const nn::pia::common::StationAddress& address) const; // 0x0072FBEC
    u32 GetSessionId() const; // 0x0072FBDC (name is ours)
    u8 GetChannel() const; // 0x0072FCF0 (name is ours)
    u32 CreateLocalCommunicationId(u32 uniqueId, bool isDemo) const; // 0x0072FF24 (name is ours)
    u32 GetBeaconApplicationDataSizeMax() const; // 0x0072FFB8 (name is ours)
    u8 GetConnectedNodeNum() const; // 0x00730F68 (name is ours)

    bool IsDuringHostMigration() const; // 0x0072FE50 | fefates:bytes [tier B]
    bool IsEnableHostMigration() const; // 0x0072FE90 | fefates:bytes [tier B]
    bool IsEnableAroundNetworkSearch() const; // 0x0072FF98 | fefates:bytes [tier B]
    bool IsHost() const; // 0x00730038 | fefates:bytes [tier B]
    bool IsClient() const; // 0x0073005C | fefates:bytes [tier B]

    // the sends of the stations are spread out (only as an extended application)
    void SleepBeforeSend(); // 0x00415838 | fefates:bytes [tier B]

    void RegisterUpdateEventCallback(nn::pia::local::LocalUpdateEventCallback callback, void* pArg); // 0x00416184 (name is ours)
    void UnregisterUpdateEventCallback(); // 0x004161C0 | fefates:bytes [tier B]
    void ProcessUpdateEventDisconnected(u8 transportId); // 0x004161D0 | fefates:bytes [tier B]
    void ProcessUpdateEventMigrationStarted(); // 0x004161EC | fefates:bytes [tier B]

    void CreateJobs(); // 0x00414AB4 | fefates:bytes-fuzzy [tier B]
    void CleanupJobs(); // 0x00414C50 | fefates:bytes-fuzzy [tier B]
    void DestroyJobs(); // 0x00414D70 | fefates:bytes [tier B]
    nn::Result InitializeCore(const nn::pia::local::UdsNetworkSetting& setting); // 0x00415418 | fefates:bytes-fuzzy [tier B]

    static bool s_IsInitialized;       // 0x00975A74 (name is ours)
    static LocalNetwork* s_pInstance;  // 0x00975A78 (name is ours)

    bool m_Unknown0x4;                                       // 0x04, vf_0x00 of the setting
    nn::pia::local::UdsNetworkSetting* m_pSetting;          // 0x08
    nn::pia::local::UdsNetworkDescription* m_pDescriptions; // 0x0C, NETWORK_DESCRIPTION_NUM
    u32 m_DescriptionNum;                                    // 0x10, of the last scan
    nn::pia::local::LocalNetworkManager* m_pNetworkManager; // 0x14
    nn::pia::local::LocalMigrationManager* m_pMigrationManager; // 0x18
    nn::pia::local::LocalAroundNetworkSearchManager* m_pAroundNetworkSearchManager; // 0x1C
    nn::pia::local::LocalCreateNetworkJob* m_pCreateNetworkJob; // 0x20
    nn::pia::local::LocalDestroyNetworkJob* m_pDestroyNetworkJob; // 0x24
    nn::pia::local::LocalScanNetworkJob* m_pScanNetworkJob; // 0x28
    nn::pia::local::LocalConnectNetworkJob* m_pConnectNetworkJob; // 0x2C
    nn::pia::local::LocalDisconnectNetworkJob* m_pDisconnectNetworkJob; // 0x30
    nn::pia::local::LocalForceDisconnectNetworkJob* m_pForceDisconnectNetworkJob; // 0x34
    nn::pia::local::LocalBackgroundProcessJob* m_pBackgroundProcessJob; // 0x38
    void* m_pReceiveBuffer;                                  // 0x3C, of uds::CTR::Initialize
    u32 m_ReceiveBufferSize;                                 // 0x40
    bool m_IsReceiveBufferExternal;                          // 0x44
    u8* m_pScanBuffer;                                       // 0x48
    u32 m_ScanBufferSize;                                    // 0x4C
    common::CriticalSection m_CriticalSection;               // 0x50
    u8 m_DisconnectReason;                                   // 0x5C
    common::CallContext m_CallContext;                       // 0x60, of the requests ...Async
    u8 m_AsyncType;                                          // 0x74
    nn::pia::local::LocalUpdateEventCallback m_UpdateEventCallback; // 0x78
    void* m_pUpdateEventCallbackArg;                         // 0x7C
    u8 m_ParticipationState;                                 // 0x80
    bool m_IsScanned;                                        // 0x81
    bool m_IsExtApplication;                                 // 0x82
    s64 m_LastSendTick;                                      // 0x88
    s64 m_SendIntervalTick;                                  // 0x90
    bool m_IsLeaveRequested;                                 // 0x98
    bool m_Unknown0x99;                                      // 0x99, set by LocalKickoutManageJob: no host migration
};
ASSERT_OFFSET(LocalNetwork, m_CriticalSection, 0x50);
ASSERT_OFFSET(LocalNetwork, m_CallContext, 0x60);
ASSERT_OFFSET(LocalNetwork, m_LastSendTick, 0x88);
ASSERT_SIZE(LocalNetwork, 0xA0);
} // namespace local
} // namespace pia
} // namespace nn
