#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/os/os_Event.h"
#include "nn/pia/common/common_CriticalSection.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"

namespace nn {
namespace pia {
namespace common {
class CallContext;
class StationAddress;
} // namespace common
namespace local {
class LocalConnectionStatus;
class LocalEventCheckBackgroundJob;
class LocalEventJob;
class LocalMessage;
class LocalNetworkDescription;
class LocalNetworkSetting;
class LocalParseSystemMessageJob;
class LocalReceiveFromJob;
class LocalSendMessageJob;
class LocalSendSystemMessageBackgroundJob;

// RTTI N2nn3pia5local19LocalNetworkManagerE
// vtable 0x00900C10 (vptr 0x00900C18), offset_to_top 0, 37 entries
//
// The node table, the messages and the jobs of the local network; UdsNetworkManager implements it
// with uds. The nodes are numbered 1 to 12 (the transport ids; 254: all, 255: none) and the
// table maps them to the node ids of the network. Messages of type 1 carry the data of the
// stations; the others are the system messages of pia local (17 update of the session, 18
// destruction, 19 start of the host migration, 20..22 / 36 / 38 search of the networks around,
// 33 answer), which go through two queues of SystemMessage. The layout is from the constructor;
// the member names and the names of the unnamed functions are ours.
class LocalNetworkManager : public ::nn::pia::common::RootObject
{
public:
    // a system message in the queues
    struct SystemMessage
    {
        static const u32 DATA_SIZE_MAX = 80;

        u8 m_Data[DATA_SIZE_MAX]; // 0x00
        u32 m_Size;               // 0x50
        u8 m_Type;                // 0x54
        u16 m_NodeId;             // 0x56, source or destination
    };

    static const u32 NODE_NUM_MAX = 12;
    static const u32 SYSTEM_MESSAGE_NUM_MAX = 12;
    static const u32 MESSAGE_BUFFER_SIZE = 1500;
    static const u8 TRANSPORT_ID_ALL = 254;
    static const u8 TRANSPORT_ID_INVALID = 255;

    // the message types
    enum MessageType : u8
    {
        MESSAGE_TYPE_DATA = 1,
        MESSAGE_TYPE_UPDATE_SESSION = 17,
        MESSAGE_TYPE_DESTROY_NETWORK = 18,
        MESSAGE_TYPE_START_HOST_MIGRATION = 19,
        MESSAGE_TYPE_AROUND_NETWORK_SEARCH_20 = 20,
        MESSAGE_TYPE_AROUND_NETWORK_SEARCH_21 = 21,
        MESSAGE_TYPE_AROUND_NETWORK_SEARCH_22 = 22,
        MESSAGE_TYPE_ACK = 33,
        MESSAGE_TYPE_AROUND_NETWORK_SEARCH_36 = 36,
        MESSAGE_TYPE_AROUND_NETWORK_SEARCH_38 = 38,
    };

    LocalNetworkManager(); // 0x00419C4C | fefates:bytes [tier B]
    virtual ~LocalNetworkManager(); // 0x00419E88 slot 0x00
    // 0x00419DEC slot 0x04 (deleting dtor)
    virtual nn::Result Initialize(nn::pia::local::LocalNetworkSetting* pSetting); // 0x004183B4 slot 0x08 | fefates:bytes [tier B]
    // the jobs and buffers are destroyed (name is ours)
    virtual void Finalize(); // 0x00419AA0 slot 0x0C
    virtual nn::Result vf_0x10() = 0; // slot 0x10
    // the connection status is read again (name is ours)
    virtual void UpdateConnectionStatus() = 0; // slot 0x14
    virtual void ProcessUpdateEvent() = 0; // slot 0x18
    virtual nn::Result SendToCore(const void* pData, u32 size, u16 nodeId) = 0; // slot 0x1C
    virtual nn::Result ReceiveFromCore(void* pBuffer, u32 bufferSize, u32* pReceivedSize, u16* pNodeId) = 0; // slot 0x20
    virtual void SetConnectionStatus(const nn::pia::local::LocalConnectionStatus* pStatus) = 0; // slot 0x24
    virtual void GetConnectionStatus(nn::pia::local::LocalConnectionStatus* pStatus) const = 0; // slot 0x28
    virtual bool IsDisconnectedByRequestFromSystem() const = 0; // slot 0x2C
    virtual u32 CreateLocalCommunicationId(u32 uniqueId, bool isDemo) const = 0; // slot 0x30
    virtual nn::Result GetApplicationData(void* pBuffer, u32* pSize, u32 bufferSize, const nn::pia::local::LocalNetworkDescription* pDescription) const = 0; // slot 0x34
    // the size of the application data in the beacon of the network (name is ours)
    virtual nn::Result GetApplicationDataSize(u32* pSize, const nn::pia::local::LocalNetworkDescription* pDescription) const = 0; // slot 0x38
    virtual const nn::pia::local::LocalBeaconSystemData* GetSystemData(const nn::pia::local::LocalNetworkDescription* pDescription) const = 0; // slot 0x3C
    // the application data in the beacon of the local network and its size (names are ours)
    virtual nn::Result GetLocalApplicationData(void* pBuffer, u32* pSize, u32 bufferSize) const = 0; // slot 0x40
    virtual nn::Result GetLocalApplicationDataSize(u32* pSize) const = 0; // slot 0x44
    virtual nn::Result SetApplicationData(const void* pData, u32 size) = 0; // slot 0x48
    virtual nn::Result MakeBeaconForCreateNetwork(nn::pia::local::LocalCreateNetworkSetting* pSetting) = 0; // slot 0x4C
    virtual u32 GetBeaconSystemDataSize() const = 0; // slot 0x50
    virtual u32 GetBeaconApplicationDataSizeMax() const = 0; // slot 0x54
    // the node of the station (name is ours)
    virtual nn::Result GetStationInfo(nn::pia::local::LocalStationInfo* pInfo, const nn::pia::common::StationAddress& address) const = 0; // slot 0x58
    virtual nn::Result GetStationInfoList(nn::pia::local::LocalStationInfo* pInfos, u8 infoNum, u32 index, const void* pScanBuffer) const = 0; // slot 0x5C
    // the link level of the wifi while connected, of a found network (names are ours)
    virtual u8 GetLinkLevel() const = 0; // slot 0x60
    virtual nn::Result GetLinkLevel(u8* pLevel, u32 index, const void* pScanBuffer) const = 0; // slot 0x64
    virtual u8 GetChannel() const = 0; // slot 0x68
    virtual nn::Result DisallowParticipating(bool isSpectatorDisallowed) = 0; // slot 0x6C
    virtual nn::Result AllowParticipating() = 0; // slot 0x70
    virtual nn::Result EjectClient(const nn::pia::common::StationAddress& address) = 0; // slot 0x74
    // all clients (name is ours)
    virtual nn::Result EjectAllClients() = 0; // slot 0x78
    virtual nn::Result CreateSessionId() = 0; // slot 0x7C
    virtual void vf_0x80(); // 0x00731184 slot 0x80
    virtual void SetupParams() = 0; // slot 0x84
    virtual nn::Result StartupImpl() = 0; // slot 0x88
    virtual void CleanupImpl() = 0; // slot 0x8C
    virtual void SetSystemDataToBeacon(void* pBeacon) = 0; // slot 0x90

    void ClearNodeList(u16 nodeId); // 0x00418808 | fefates:bytes [tier B]
    void ClearNodeList(); // 0x00418844 | fefates:bytes [tier B]
    // the data of a station or a system message (into the queue); RESULT_NO_DATA without data
    // (name is ours)
    nn::Result ReceiveFrom(void* pBuffer, u32 bufferSize, u32* pSize, u8* pTransportId, bool isSystemMessageOnly); // 0x00418568
    void SetSessionId(u32 sessionId); // 0x004187FC (name is ours)
    // a new random value for the update messages (name is ours)
    void RenewUnknown0x123C(); // 0x00418858
    // a message to a station or to all (TRANSPORT_ID_ALL) (name is ours)
    nn::Result SendTo(const void* pData, u8 type, u32 size, u8 transportId); // 0x00418888
    nn::Result SendTo(const void* pData, u32 size, u8 transportId); // 0x00418984 | fefates:bytes [tier B]
    // a system message into the send queue (name is ours)
    nn::Result PushSendSystemMessage(const void* pData, u8 type, u32 size, u16 nodeId); // 0x00418B04
    bool IsActiveLocalInputStream(); // 0x00418C48 | fefates:bytes [tier B]
    // the time of the received data (the linker put it in front of common::CriticalSection::
    // Unlock, into which it falls; name is ours)
    void SetReceiveTime(const common::Time& time); // 0x00427368
    // the host sends the node table to all (name is ours)
    void SendUpdateSessionMessage(); // 0x00418CC4
    void ParseUpdateSessionMessage(const SystemMessage* pMessage); // 0x00418E1C | fefates:bytes [tier B]
    // the host tells the clients that the network ends / the host migration starts (names are
    // ours)
    void SendDestroyNetworkMessage(); // 0x00419210
    void SendStartHostMigrationMessage(); // 0x00419710
    // the system messages of the send queue go out / those of the receive queue are processed
    // (names are ours)
    void SendSystemMessages(); // 0x0041927C
    void ParseSystemMessages(); // 0x004193D0
    nn::Result SendTo(const nn::pia::local::LocalMessage* pMessage, u8 transportId); // 0x004196E8 | fefates:bytes [tier B]
    void Cleanup(); // 0x0041977C (name is ours)
    nn::Result Startup(); // 0x004198C8 | fefates:bytes [tier B]
    void ClearIds(); // 0x00419A4C | fefates:bytes [tier B]
    // the number of the nodes in the table
    u8 GetNodeNum() const; // 0x00730EAC | fefates:bytes [tier B]
    u8 GetApplicationVersion() const; // 0x00730EDC | fefates:bytes [tier B]
    // the application data part of the beacon (without the system data of pia)
    void RemoveBeaconSystemData(void* pBuffer, const void* pBeacon, u32 size) const; // 0x00730EFC (name after LocalMigrationManager's)
    // the number of the nodes in the connection status (name is ours)
    u8 GetConnectedNodeNum(); // 0x00730F70
    u32 GetConnectedTransportIdBitmap(bool isLocalExcluded) const; // 0x00730FB4 | fefates:bytes [tier B]
    u8 ConvertLocalNodeIdToTransportId(u16 nodeId) const; // 0x00731024 | fefates:bytes [tier B]
    u16 ConvertTransportIdToLocalNodeId(u8 transportId) const; // 0x007310A0 | fefates:bytes [tier B]
    bool IsWaitingUpdateSessionAckMessage() const; // 0x00731108 | fefates:bytes [tier B]
    u32 GetConnectedTransportIdBitmapFromHost(bool isLocalExcluded) const; // 0x00731120 | fefates:bytes [tier B]
    bool IsHost() const; // 0x00731188 | fefates:bytes [tier B]
    bool IsClient() const; // 0x007311AC | fefates:bytes [tier B]

    nn::os::Event m_StatusEvent;                     // 0x0004, of uds::CTR::Initialize
    nn::pia::local::LocalNetworkSetting* m_pSetting; // 0x0008
    u16 m_SessionVersion;                            // 0x000C, of the update messages
    u8 m_LocalTransportId;                           // 0x000E
    u8 m_HostTransportId;                            // 0x000F
    u16 m_NodeIds[NODE_NUM_MAX];                     // 0x0010, by transport id - 1
    u16 m_HostNodeIds[NODE_NUM_MAX];                 // 0x0028, of the last update message
    u16 m_LocalNodeId;                               // 0x0040
    u16 m_InvalidNodeId;                             // 0x0042
    u16 m_BroadcastNodeId;                           // 0x0044
    common::CriticalSection m_SystemSendCriticalSection; // 0x0048
    common::CriticalSection m_SendCriticalSection;   // 0x0054
    common::CriticalSection m_ReceiveCriticalSection; // 0x0060
    u8 m_SystemSendBuffer[MESSAGE_BUFFER_SIZE];      // 0x006C
    u8 m_SendBuffer[MESSAGE_BUFFER_SIZE];            // 0x0648
    u8 m_ReceiveBuffer[MESSAGE_BUFFER_SIZE];         // 0x0C24
    nn::pia::local::LocalSendMessageJob* m_pSendMessageJob; // 0x1200
    common::CriticalSection m_ConnectionStatusCriticalSection; // 0x1204
    nn::pia::local::LocalConnectionStatus* m_pConnectionStatus; // 0x1210
    common::CriticalSection m_ApplicationDataCriticalSection; // 0x1214
    u8* m_pApplicationData;                          // 0x1220, beacon (system and application data)
    u32 m_ApplicationDataSize;                       // 0x1224
    common::CriticalSection m_SystemDataCriticalSection; // 0x1228
    nn::pia::local::LocalBeaconSystemData* m_pSystemData; // 0x1234
    u32 m_Unknown0x1238;                             // 0x1238, the previous m_Unknown0x123C
    u32 m_Unknown0x123C;                             // 0x123C
    u32 m_SessionId;                                 // 0x1240
    u32 m_DisconnectedTransportIdBitmap;             // 0x1244, reported with the next update
    nn::pia::local::LocalEventJob* m_pEventJob;      // 0x1248
    nn::pia::local::LocalEventCheckBackgroundJob* m_pEventCheckBackgroundJob; // 0x124C
    nn::pia::local::LocalReceiveFromJob* m_pReceiveFromJob; // 0x1250
    nn::pia::local::LocalSendSystemMessageBackgroundJob* m_pSendSystemMessageBackgroundJob; // 0x1254
    nn::pia::local::LocalParseSystemMessageJob* m_pParseSystemMessageJob; // 0x1258
    common::CallContext* m_pCallContext;             // 0x125C
    common::CriticalSection m_ReceiveTimeCriticalSection; // 0x1260
    common::Time m_ReceiveTime;                      // 0x1270, of the last received data
    bool m_Unknown0x1278;                            // 0x1278
    u32 m_HostTransportIdBitmap;                     // 0x127C, connected stations after the host
    common::CriticalSection m_SendQueueCriticalSection; // 0x1280
    SystemMessage m_SendQueue[SYSTEM_MESSAGE_NUM_MAX]; // 0x128C
    SystemMessage m_SendQueueCopy[SYSTEM_MESSAGE_NUM_MAX]; // 0x16AC
    u32 m_SendQueueNum;                              // 0x1ACC
    bool m_IsSendQueueUpdated;                       // 0x1AD0
    common::CriticalSection m_ReceiveQueueCriticalSection; // 0x1AD4
    SystemMessage m_ReceiveQueue[SYSTEM_MESSAGE_NUM_MAX]; // 0x1AE0
    u32 m_ReceiveQueueNum;                           // 0x1F00
    bool m_IsReceiveQueueUpdated;                    // 0x1F04
    common::CriticalSection m_EventCriticalSection;  // 0x1F08
    bool m_IsEventSignaled;                          // 0x1F14
    bool m_IsSessionStarted;                         // 0x1F15, waits for the first update message
};
ASSERT_OFFSET(LocalNetworkManager, m_LocalNodeId, 0x40);
ASSERT_OFFSET(LocalNetworkManager, m_pSendMessageJob, 0x1200);
ASSERT_OFFSET(LocalNetworkManager, m_pSystemData, 0x1234);
ASSERT_OFFSET(LocalNetworkManager, m_pCallContext, 0x125C);
ASSERT_OFFSET(LocalNetworkManager, m_SendQueue, 0x128C);
ASSERT_OFFSET(LocalNetworkManager, m_ReceiveQueue, 0x1AE0);
ASSERT_OFFSET(LocalNetworkManager, m_IsSessionStarted, 0x1F15);
} // namespace local
} // namespace pia
} // namespace nn
