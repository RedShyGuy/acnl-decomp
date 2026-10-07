#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_SignatureSetting.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_ProtocolId.h"
#include "nn/pia/transport/transport_Station.h"

namespace nn {
namespace pia {
namespace common {
class CryptoSetting;
class MonitoringDataSender;
class StationAddress;
} // namespace common
namespace transport {
class BandwidthCheckerProtocol;
class MissingStationHandler;
class NetworkFactory;
class Station;
class StationConnectionInfo;
class StationProtocol;
} // namespace transport
namespace session {
class CreateMeshJob;
class DestroyMeshJob;
class JoinMeshJob;
class KickoutManageJob;
class LeaveMeshJob;
class LeaveWithHostMigrationJob;
class MeshEventListener;
class MeshProtocol;
class ProcessDestroyMeshJob;
class ProcessHostMigrationJob;
class ProcessJoinRequestJob;
class ProcessUpdateMeshJob;
class RelayRouteManageJob;
class SignatureSettingStorage;

// RTTI N2nn3pia7session4MeshE @ 0x008D0140
// vtable 0x00901D28 (vptr 0x00901D30), offset_to_top 0, 3 entries
//
// The mesh: the stations of the session, the host, the jobs that create / join / leave / destroy
// it and the protocols of the session layer. One instance (CreateInstance). The layout is from
// the constructor, Initialize and Startup; the member names, the enumerators and the names
// marked so are ours.
class Mesh : public ::nn::pia::common::RootObject
{
public:
    // the argument of CreateInstance (names are ours)
    struct Setting
    {
        transport::NetworkFactory* m_pNetworkFactory; // 0x0
        u8 m_RelayMode;                               // 0x4, 1, 2: with relay routes
        bool m_IsBandwidthCheckEnabled;               // 0x5
    };

    // the argument of Startup (the fefates symbols have its members as parameters; the names
    // are ours)
    struct StartupSetting
    {
        // the defaults (inline in MeshLayerController::StartupMesh)
        StartupSetting()
            : m_TimeoutMSec(10000), m_KeepAliveIntervalMSec(1000), m_IsHostMigrationEnabled(true), m_pIdentificationData(nullptr),
              m_pCryptoSetting(nullptr), m_SignatureSetting(common::g_DefaultSignatureSetting), m_BandwidthCheckBandwidth(-1),
              m_BandwidthCheckPacketSize(0), m_BandwidthCheckDurationMSec(1000), m_IsBandwidthCheckOneWay(false), m_pPlayerName(nullptr)
        {
        }

        u32 m_TimeoutMSec;                          // 0x00, 1000..29999
        u32 m_KeepAliveIntervalMSec;                // 0x04
        bool m_IsHostMigrationEnabled;              // 0x08
        const u8* m_pIdentificationData;            // 0x0C, 32 bytes, then the length and one more byte
        const common::CryptoSetting* m_pCryptoSetting; // 0x10
        // (SignatureSettingStorage; its key size goes into the monitoring data)
        common::SignatureSetting m_SignatureSetting; // 0x14
        s32 m_BandwidthCheckBandwidth;              // 0x24 (BandwidthCheckerProtocol::Setup)
        u32 m_BandwidthCheckPacketSize;             // 0x28
        s32 m_BandwidthCheckDurationMSec;           // 0x2C
        bool m_IsBandwidthCheckOneWay;              // 0x30
        const transport::Station::PlayerName* m_pPlayerName; // 0x34
    };

    // the asynchronous operation of m_CallContext (m_AsyncType)
    enum AsyncType : u8
    {
        ASYNC_TYPE_NONE = 0,
        ASYNC_TYPE_CREATE = 1,
        ASYNC_TYPE_DESTROY = 2,
        ASYNC_TYPE_LEAVE_WITH_HOST_MIGRATION = 3,
        ASYNC_TYPE_JOIN = 4,
        ASYNC_TYPE_LEAVE = 5,
    };

    // the type name is from the signature of NoticeMeshEvent
    enum EventType : u8
    {
        EVENT_TYPE_JOIN = 0,
        EVENT_TYPE_LEAVE = 1,
        EVENT_TYPE_HOST_CHANGED = 2,          // ProcessHostMigrationJob
        EVENT_TYPE_HOST_MIGRATION_FAILED = 3, // ProcessHostMigrationJob
        EVENT_TYPE_MESH_JOINED = 17,
        EVENT_TYPE_MESH_LEFT = 18,
        EVENT_TYPE_19 = 19, // a host migration started while joining (ProcessHostMigrationJob)
        // from JoinMeshJob: response code 2, a failed connection, the join response arrived (with
        // bytes 8 to 10 of it)
        EVENT_TYPE_20 = 20,
        EVENT_TYPE_GREETING = 21, // a station greets the new host (MeshProtocol)
        EVENT_TYPE_CONNECTION_FAILED = 22,
        EVENT_TYPE_JOIN_RESPONSE = 23,
    };

    // the type name is from the signature of MonitoringProcess
    enum DisconnectReason : u8
    {
        DISCONNECT_REASON_NONE = 0,
        DISCONNECT_REASON_1 = 1,
        DISCONNECT_REASON_LEAVE = 2,
        DISCONNECT_REASON_BY_HOST = 3, // kicked out (KickoutManageJob) or the host destroyed the mesh
        DISCONNECT_REASON_KICKOUT_4 = 4,
        DISCONNECT_REASON_KICKOUT_5 = 5,
        DISCONNECT_REASON_6 = 6, // the station data list is not from the host (MeshProtocol), an update failed
        DISCONNECT_REASON_HOST_MIGRATION_FAILED = 7, // ProcessHostMigrationJob
        DISCONNECT_REASON_8 = 8, // the host is gone and there is no host migration (MeshProtocol)
        DISCONNECT_REASON_9 = 9,
    };

    // what m_pEventListener gets (name is ours)
    struct Event
    {
        EventType m_Type;          // 0x0
        StationIndex m_StationIndex; // 0x1
        u32 m_Unknown0x4;          // 0x4
    };
    // the callback of the station events (name is ours)
    typedef void (*EventCallback)(EventType type, StationId stationId);
    // returns the value of the identification info at 0x48 (name is ours)
    typedef u32 (*IdentificationCallback)();
    // decides on a join request after the identification info of the station (ProcessJoinRequestJob;
    // name is ours)
    typedef bool (*JoinApprovalCallback)(const transport::Station::IdentificationInfo* pInfo);
    // the priority of a station as the next host, the highest byte of its rank in
    // ProcessHostMigrationJob::MakeHostCandidateRanking (name is ours)
    typedef u32 (*HostCandidateCallback)(StationIndex stationIndex, bool isFromConnectionInfo);

    // the setting of the next CreateInstance (two bytes at 0x0097E444; name is ours)
    struct GlobalSetting
    {
        bool m_IsRelayRouteNetwork; // 0x0 (RelayRouteManager::Initialize)
        bool m_IsTimeoutFree;       // 0x1, Startup does not check the timeout
    };

    Mesh(); // 0x00449D4C
    virtual void Trace(u64 flag) const; // 0x00734334 slot 0x00
    virtual ~Mesh(); // 0x00449E50 slot 0x04
    // 0x00449E40 slot 0x08 (deleting dtor)

    static nn::Result CreateInstance(const Setting& setting); // 0x00448964 (name is ours)
    static void DestroyInstance(); // 0x00448C58 | fefates:bytes
    // (name is ours)
    static nn::Result SetGlobalSetting(const GlobalSetting& setting); // 0x0044973C
    nn::Result Initialize(const Setting& setting, bool isRelayRouteNetwork); // 0x00447C70 (name is ours)
    nn::Result Startup(const StartupSetting& setting); // 0x004498B0 | fefates:callgraph
    void Cleanup(); // 0x00449860 | fefates:callgraph
    void CleanupJobs(); // 0x004480D8 | fefates:bytes-fuzzy [tier B]
    void CleanupStatus(); // 0x004485A4 | fefates:callgraph
    void CleanupStationsJobs(); // 0x004491D8 | fefates:bytes [tier B]
    nn::Result SetupProtocols(); // 0x00448BE0 | fefates:callgraph

    nn::Result CreateMesh(nn::pia::common::CallContext* pCallContext); // 0x00447BCC | fefates:callgraph
    nn::Result DestroyMesh(nn::pia::common::CallContext* pCallContext); // 0x00448280 | fefates:callgraph
    DECOMP_NOINLINE nn::Result joinMeshCore(const nn::pia::transport::StationConnectionInfo& info, nn::pia::common::CallContext* pCallContext); // 0x004483F4 | fefates:callgraph
    nn::Result LeaveMesh(nn::pia::common::CallContext* pCallContext); // 0x00449C5C | fefates:callgraph
    // (name is ours)
    nn::Result LeaveMeshWithHostMigration(nn::pia::common::CallContext* pCallContext); // 0x00449634

    // the same with the own call context (names are ours except the fefates ones)
    nn::Result CreateMeshAsync(); // 0x00448C00 | fefates:callgraph
    nn::Result DestroyMeshAsync(); // 0x00448DF0 | fefates:callgraph
    nn::Result JoinMeshAsync(const nn::pia::transport::StationConnectionInfo& info); // 0x00448878
    nn::Result LeaveMeshAsync(); // 0x00448B88 | fefates:callgraph
    nn::Result LeaveMeshWithHostMigrationAsync(); // 0x00449770
    nn::Result GetCreateMeshAsyncResult() const; // 0x0044921C
    nn::Result GetDestroyMeshAsyncResult() const; // 0x00449374
    nn::Result GetLeaveMeshWithHostMigrationAsyncResult() const; // 0x004497C8
    nn::Result GetJoinMeshAsyncResult() const; // 0x00448FAC
    nn::Result GetLeaveMeshAsyncResult() const; // 0x00449174
    bool IsCreateMeshAsyncCompleted() const; // 0x0044941C
    bool IsDestroyMeshAsyncCompleted() const; // 0x004494A0
    bool IsLeaveMeshWithHostMigrationAsyncCompleted() const; // 0x004497FC
    bool IsJoinMeshAsyncCompleted() const; // 0x00449264
    bool IsLeaveMeshAsyncCompleted() const; // 0x004493A8
    nn::Result CancelJoinMeshAsync(); // 0x004491A8

    // the station joined / left
    void FixConnectedId(nn::pia::StationIndex stationIndex); // 0x00448AB4 | fefates:bytes-fuzzy [tier B]
    void UnfixDisconnectedId(nn::pia::StationIndex stationIndex); // 0x0044929C | fefates:bytes-fuzzy [tier B]
    void StartUse(nn::pia::StationIndex stationIndex); // 0x00449C04 | fefates:bytes-fuzzy [tier B]
    void NoticeMeshEvent(nn::pia::session::Mesh::EventType type, nn::pia::StationIndex stationIndex); // 0x00448CD8 | fefates:bytes-fuzzy [tier B]
    void NotifyLeaveStationAddress(const nn::pia::common::StationAddress& address); // 0x004494E8 | fefates:bytes [tier B]
    // 0: no, 2: the mesh does not take stations, 255: by the station id table
    u8 CheckApprovalJoin(nn::pia::transport::Station* pStation); // 0x00448E48 | fefates:callgraph
    bool CheckStationIndexIsValid(nn::pia::StationIndex stationIndex) const; // 0x007342F0 | fefates:callgraph
    // (names are ours)
    // the phase of the JoinMeshJob (0 without one)
    u8 GetJoinMeshJobPhase() const; // 0x00449250
    // why the local station is not in the mesh: NONE while in it, KICKOUT_4 / KICKOUT_5 after a
    // kickout notice (name is ours)
    DisconnectReason GetDisconnectReason() const; // 0x00734218
    // RESULT_NOT_JOINED if not in the mesh
    nn::Result CheckJoined() const; // 0x0073425C
    // the first index without a station (UNIDENTIFIED if all are taken; name is ours)
    StationIndex GetFreeStationIndex() const; // 0x004493E0
    // the time of the SyncClockProtocol in ms (-3: no transport, -2: no protocol, -1: not synchronized)
    s64 GetTime() const; // 0x00734338 | fefates:bytes-fuzzy [tier B]
    // the local station is the host of the mesh (name is ours)
    static bool IsLocalHost(); // 0x00734394
    // the flag of the monitoring data sender (MonitoringDataSender slot 0x0C; name is ours)
    bool IsMonitoringDataSenderFlagSet() const; // 0x007342C4
    // a leave or destroy job runs (name is ours)
    bool IsLeaving() const; // 0x00734274

    // the monitoring data
    DECOMP_NOINLINE void MonitoringProcess(nn::pia::session::Mesh::DisconnectReason reason, unsigned char phase); // 0x00448FEC | fefates:callgraph
    // (names are ours)
    void UpdateMonitoringData(); // 0x00448FE0
    void EndMonitoring(DisconnectReason reason); // 0x00449294
    void SetSessionBeginMonitoringData(); // 0x0044944C
    void SendMonitoringData(bool isSessionEnd); // 0x004480B0
    void ClearSessionBeginMonitoringData(); // 0x00449584

    // (names are ours)
    void SetJoined(bool isJoined); // 0x00448394
    void Disconnect(DisconnectReason reason); // 0x004488D8
    void SetHostMigrationStartFlag(bool flag); // 0x00449534 | fefates:callgraph
    void SetSyncClockRequestInterval(s32 intervalMSec); // 0x0044953C
    void SetUnknown0xA5(bool value); // 0x004493D8
    void ClearStationBitmap(); // 0x004494D0
    void ClearEventListener(); // 0x004494DC
    void ClearIdentificationCallback(); // 0x00449764
    void ClearJoinApprovalCallback(); // 0x0044982C
    nn::Result SetHostCandidateCallback(HostCandidateCallback callback); // 0x00449838
    void ClearHostCandidateCallback(); // 0x00449854

    static Mesh* s_pInstance;

    CreateMeshJob* m_pCreateMeshJob;                         // 0x04
    JoinMeshJob* m_pJoinMeshJob;                             // 0x08
    LeaveMeshJob* m_pLeaveMeshJob;                           // 0x0C
    ProcessJoinRequestJob* m_pProcessJoinRequestJob;         // 0x10
    ProcessUpdateMeshJob* m_pProcessUpdateMeshJob;           // 0x14
    DestroyMeshJob* m_pDestroyMeshJob;                       // 0x18
    ProcessDestroyMeshJob* m_pProcessDestroyMeshJob;         // 0x1C
    ProcessHostMigrationJob* m_pProcessHostMigrationJob;     // 0x20
    LeaveWithHostMigrationJob* m_pLeaveWithHostMigrationJob; // 0x24
    SignatureSettingStorage* m_pSignatureSettingStorage;     // 0x28
    RelayRouteManageJob* m_pRelayRouteManageJob;             // 0x2C
    KickoutManageJob* m_pKickoutManageJob;                   // 0x30
    transport::MissingStationHandler* m_pMissingStationHandler; // 0x34
    EventCallback m_EventCallback;                           // 0x38
    JoinApprovalCallback m_JoinApprovalCallback;             // 0x3C
    HostCandidateCallback m_HostCandidateCallback;           // 0x40
    IdentificationCallback m_IdentificationCallback;         // 0x44
    transport::ProtocolId m_MeshProtocolId;                  // 0x48
    MeshProtocol* m_pMeshProtocol;                           // 0x4C
    transport::StationProtocol* m_pStationProtocol;          // 0x50
    transport::ProtocolId m_BandwidthCheckerProtocolId;      // 0x54
    transport::BandwidthCheckerProtocol* m_pBandwidthCheckerProtocol; // 0x58
    u16 m_StationNum;                                        // 0x5C
    u16 m_StationNumMax;                                     // 0x5E
    bool m_IsJoinable;                                       // 0x60
    bool m_Unknown0x61;                                      // 0x61
    StationIndex m_HostStationIndex;                         // 0x62
    StationIndex m_LocalStationIndex;                        // 0x63
    bool m_IsJoined;                                         // 0x64
    DisconnectReason m_DisconnectReason;                     // 0x65
    u8 m_HostMigrationMode;                                  // 0x66, 1, 2: with host migration
    u8 m_RelayMode;                                          // 0x67
    bool m_IsBandwidthCheckEnabled;                          // 0x68
    bool m_IsHostMigrationEnabled;                           // 0x69
    u32 m_StationBitmap;                                     // 0x6C
    u32 m_Unknown0x70;                                       // 0x70 (0xFFFF)
    transport::ProtocolId m_SyncClockProtocolId;             // 0x74
    common::Time m_MonitoringStartTime;                      // 0x78
    bool m_IsMonitoring;                                     // 0x80
    bool m_IsStarted;                                        // 0x81
    bool m_HostMigrationStartFlag;                           // 0x82
    bool m_Unknown0x83;                                      // 0x83
    common::MonitoringDataSender* m_pMonitoringDataSender;   // 0x84
    common::CallContext m_CallContext;                       // 0x88, of the *Async functions
    AsyncType m_AsyncType;                                   // 0x9C
    MeshEventListener* m_pEventListener;                         // 0xA0
    bool m_IsMonitoringDataSent;                             // 0xA4
    bool m_Unknown0xA5;                                      // 0xA5
};
ASSERT_OFFSET(Mesh, m_EventCallback, 0x38);
ASSERT_OFFSET(Mesh, m_StationNum, 0x5C);
ASSERT_OFFSET(Mesh, m_StationBitmap, 0x6C);
ASSERT_OFFSET(Mesh, m_MonitoringStartTime, 0x78);
ASSERT_OFFSET(Mesh, m_CallContext, 0x88);
ASSERT_SIZE(Mesh, 0xA8);
} // namespace session
} // namespace pia
} // namespace nn
