#pragma once

#include "decomp.h"
#include "nn/cfg/CTR/cfg_Types.h"
#include "nn/pia/local/local_LocalNetworkManager.h"
#include "nn/pia/local/local_UdsHandle.h"
#include "nn/uds/CTR/uds_Types.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local17UdsNetworkManagerE @ 0x008CFB68
// vtable 0x00900AA8 (vptr 0x00900AB0), offset_to_top 0, 39 entries
//
// The node table and the messages of the local network with uds: the node ids are the ones of
// uds (1 is the host, 0 none, 0xFFFF all), the data go over the endpoints of UdsHandle and the
// system data of pia sit in front of the application data of the beacon. The layout is from the
// constructor; the member names and the names of the unnamed functions are ours.
class UdsNetworkManager : public ::nn::pia::local::LocalNetworkManager
{
public:
    static const u16 DATA_CHANNEL = 0x23F3;
    static const u32 BEACON_DATA_SIZE = 200;
    static const u32 NODE_INFORMATION_NUM = 16;

    // uds::CTR::ConnectionStatus::status
    enum Status
    {
        STATUS_DISCONNECTED = 3,
        STATUS_HOST = 6,
        STATUS_CLIENT = 9,
        STATUS_SPECTATOR = 10,
        STATUS_DESTROYED = 11,
    };

    UdsNetworkManager(); // 0x00417FDC | fefates:bytes [tier B]
    virtual ~UdsNetworkManager(); // 0x00418090 slot 0x00
    // 0x00418058 slot 0x04 (deleting dtor)
    virtual nn::Result Initialize(nn::pia::local::LocalNetworkSetting* pSetting); // 0x00416DC8 slot 0x08
    virtual void Finalize(); // 0x00417F20 slot 0x0C
    virtual nn::Result vf_0x10(); // 0x00417C40 slot 0x10
    virtual void UpdateConnectionStatus(); // 0x0041709C slot 0x14
    virtual void ProcessUpdateEvent(); // 0x004174D4 slot 0x18
    virtual nn::Result SendToCore(const void* pData, u32 size, u16 nodeId); // 0x00416F2C slot 0x1C
    virtual nn::Result ReceiveFromCore(void* pBuffer, u32 bufferSize, u32* pReceivedSize, u16* pNodeId); // 0x0041718C slot 0x20
    virtual void SetConnectionStatus(const nn::pia::local::LocalConnectionStatus* pStatus); // 0x00417B08 slot 0x24
    virtual void GetConnectionStatus(nn::pia::local::LocalConnectionStatus* pStatus) const; // 0x007307A8 slot 0x28
    virtual bool IsDisconnectedByRequestFromSystem() const; // 0x00730E14 slot 0x2C
    // (the linker put it in front of uds::CTR::CreateLocalCommunicationId, into which it falls)
    virtual u32 CreateLocalCommunicationId(u32 uniqueId, bool isDemo) const; // 0x00468B04 slot 0x30
    virtual nn::Result GetApplicationData(void* pBuffer, u32* pSize, u32 bufferSize, const nn::pia::local::LocalNetworkDescription* pDescription) const; // 0x007306B4 slot 0x34
    virtual nn::Result GetApplicationDataSize(u32* pSize, const nn::pia::local::LocalNetworkDescription* pDescription) const; // 0x0073091C slot 0x38
    virtual const nn::pia::local::LocalBeaconSystemData* GetSystemData(const nn::pia::local::LocalNetworkDescription* pDescription) const; // 0x007302E8 slot 0x3C
    virtual nn::Result GetLocalApplicationData(void* pBuffer, u32* pSize, u32 bufferSize) const; // 0x00730B18 slot 0x40
    virtual nn::Result GetLocalApplicationDataSize(u32* pSize) const; // 0x00730CE0 slot 0x44
    virtual nn::Result SetApplicationData(const void* pData, u32 size); // 0x00417E04 slot 0x48
    virtual nn::Result MakeBeaconForCreateNetwork(nn::pia::local::LocalCreateNetworkSetting* pSetting); // 0x00417D6C slot 0x4C
    virtual u32 GetBeaconSystemDataSize() const; // 0x007309DC slot 0x50
    virtual u32 GetBeaconApplicationDataSizeMax() const; // 0x00730C98 slot 0x54
    virtual nn::Result GetStationInfo(nn::pia::local::LocalStationInfo* pInfo, const nn::pia::common::StationAddress& address) const; // 0x0073039C slot 0x58
    virtual nn::Result GetStationInfoList(nn::pia::local::LocalStationInfo* pInfos, u8 infoNum, u32 index, const void* pScanBuffer) const; // 0x0041730C slot 0x5C
    virtual u8 GetLinkLevel() const; // 0x007302B8 slot 0x60
    virtual nn::Result GetLinkLevel(u8* pLevel, u32 index, const void* pScanBuffer) const; // 0x00730590 slot 0x64
    virtual u8 GetChannel() const; // 0x00730674 slot 0x68
    virtual nn::Result DisallowParticipating(bool isSpectatorDisallowed); // 0x00417B64 slot 0x6C
    virtual nn::Result AllowParticipating(); // 0x00417238 slot 0x70
    virtual nn::Result EjectClient(const nn::pia::common::StationAddress& address); // 0x00416FD4 slot 0x74
    virtual nn::Result EjectAllClients(); // 0x004170D8 slot 0x78
    virtual nn::Result CreateSessionId(); // 0x00417118 slot 0x7C
    // (a nop that falls into the one of LocalNetworkManager)
    virtual void vf_0x80(); // 0x00731180 slot 0x80
    virtual void SetupParams(); // 0x0041701C slot 0x84
    virtual nn::Result StartupImpl(); // 0x00417040 slot 0x88
    // (the linker put it in front of uds::CTR::Finalize, into which it falls)
    virtual void CleanupImpl(); // 0x00469ED4 slot 0x8C
    virtual void SetSystemDataToBeacon(void* pBeacon); // 0x00417BA0 slot 0x90
    // a node came / left (the index into the node ids of the status)
    virtual void ProcessConnectEvent(const nn::uds::CTR::ConnectionStatus& status, u32 index); // 0x004178B8 slot 0x94
    virtual void ProcessDisconnectEvent(const nn::uds::CTR::ConnectionStatus& status, u32 index); // 0x00417C48 slot 0x98

    nn::Result CreateSystemHandle(); // 0x00417270
    nn::Result DestroySystemHandle(); // 0x0041784C
    nn::Result ConvertUdsResult(const nn::Result& result) const; // 0x00730480
    nn::Result ConvertUdsSendToResult(const nn::Result& result) const; // 0x00730804
    nn::Result ConvertUdsReceiveFromResult(const nn::Result& result) const; // 0x00730A0C
    u8 ConvertLocalNodeIdToStationInfoRole(u16 nodeId) const; // 0x00730E50

    u16 m_NodeBitmap;                               // 0x1F16, of the last connection status
    nn::pia::local::UdsHandle m_Handle;             // 0x1F18
    common::CriticalSection m_HandleCriticalSection; // 0x1F28
    nn::cfg::CTR::UserName* m_pUserName;            // 0x1F34, of uds::CTR::Initialize
    nn::uds::CTR::NodeInformation m_NodeInformations[NODE_INFORMATION_NUM]; // 0x1F38, of GetStationInfoList
};
ASSERT_OFFSET(UdsNetworkManager, m_Handle, 0x1F18);
ASSERT_OFFSET(UdsNetworkManager, m_pUserName, 0x1F34);
ASSERT_SIZE(UdsNetworkManager, 0x21B8);
} // namespace local
} // namespace pia
} // namespace nn
