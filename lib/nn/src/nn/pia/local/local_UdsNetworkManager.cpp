#include "nn/pia/local/local_UdsNetworkManager.h"
#include "nn/nwm/CTR/CTR_Api.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager.h"
#include "nn/pia/local/local_LocalMigrationManager.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalSendMessageJob.h"
#include "nn/pia/local/local_UdsMigrationManagerNew.h"
#include "nn/pia/local/local_UdsNetworkConnectionStatus.h"
#include "nn/pia/local/local_UdsNetworkDescription.h"
#include "nn/pia/local/local_UdsNetworkSetting.h"
#include "nn/uds/CTR/CTR_Api.h"
#include "nn/uds/CTR/detail/detail_Api.h"
#include "nn/uds/CTR/uds_NetworkDescriptionReader.h"
#include "nn/uds/CTR/uds_Result.h"
#include "nn/uds/CTR/uds_ScanResultReader.h"
#include "pead/peadHashCrc32.h"
#include "pead/peadHeapMgr.h"
#include "pead/peadRandom.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
namespace {
const u16 NODE_ID_HOST = 1;
const u16 NODE_ID_BROADCAST = 0xFFFF;
// the session id is the CRC-32 of the MAC address and two random bytes
const u32 SESSION_ID_SOURCE_SIZE = 8;
const u32 MAC_ADDRESS_SIZE = 6;

// more uds results the conversions tell apart (names after level/summary/description)
const bit32 RESULT_UDS_OUT_OF_RANGE_1018 = 0xC90113FA;       // status, wrong argument, 1018
const bit32 RESULT_UDS_OUT_OF_RESOURCE_1008 = 0xC86113F0;    // status, out of resource, 1008

// the description of the last disconnection that pia keeps (LocalNetwork::m_DisconnectReason)
inline u8 ConvertDisconnectReason(u32 reason)
{
    if (reason == 0 || reason == 1 || reason == 2 || reason == 3 || reason == 4 || reason == 5) {
        return static_cast<u8>(reason);
    }
    return 6;
}

// the permanent invalid state of nwm (module 27, description 6) that uds passes on
inline bool IsNwmInvalidState(const nn::Result& result)
{
    s32 level = static_cast<s32>(result.GetPrintableBits()) >> 27;
    return level == -5 && result.GetModule() == 27 && result.GetSummary() == 5 && result.GetDescription() == 6;
}

// the failure of a uds call that changes the network
inline nn::Result ConvertNetworkResult(const UdsNetworkManager* pManager, const nn::Result& result)
{
    if (result == nn::uds::CTR::RESULT_NOT_AUTHORIZED_STATE) {
        return common::RESULT_LOCAL_NETWORK_LOST;
    }
    return pManager->ConvertUdsResult(result);
}

inline void CopyStationInfo(nn::pia::local::LocalStationInfo* pInfo, const nn::uds::CTR::NodeInformation& information, u8 role)
{
    std::memcpy(pInfo->m_ScrambledLocalFriendCode, &information.scrambledLocalFriendCode, sizeof(pInfo->m_ScrambledLocalFriendCode));
    std::memcpy(pInfo->m_UserName, &information.userName, sizeof(pInfo->m_UserName));
    pInfo->m_Role = role;
}
} // namespace

// 0x00416DC8
nn::Result nn::pia::local::UdsNetworkManager::Initialize(nn::pia::local::LocalNetworkSetting* pSetting)
{
    LocalNetworkManager::Initialize(pSetting);
    const UdsNetworkSetting* pUdsSetting = static_cast<const UdsNetworkSetting*>(m_pSetting);
    if (m_pConnectionStatus == nullptr) {
        m_pConnectionStatus = common::NewObject<UdsNetworkConnectionStatus>();
    }
    if (m_pUserName == nullptr && pUdsSetting->m_pUserName != nullptr) {
        m_pUserName = common::NewObject<nn::cfg::CTR::UserName>();
        std::memcpy(m_pUserName, pUdsSetting->m_pUserName, sizeof(nn::cfg::CTR::UserName));
    }
    if (m_pSystemData == nullptr) {
        // with the host migration the system data are larger
        if (pSetting->m_IsHostMigrationEnabled) {
            m_pSystemData = common::NewObject<UdsMigrationManagerNew::BeaconSystemData>();
        } else {
            m_pSystemData = common::NewObject<LocalBeaconSystemData>();
        }
    }
    return nn::Result();
}

// 0x00416F2C
nn::Result nn::pia::local::UdsNetworkManager::SendToCore(const void* pData, u32 size, u16 nodeId)
{
    m_HandleCriticalSection.Lock();
    if (!m_Handle.m_IsCreated) {
        m_HandleCriticalSection.Unlock();
        return common::RESULT_LOCAL_NETWORK_LOST;
    }
    const UdsNetworkSetting* pSetting = static_cast<const UdsNetworkSetting*>(m_pSetting);
    nn::Result result = nn::uds::CTR::SendTo(m_Handle.m_SendEndpoint, pData, size, nodeId, static_cast<u8>(m_Handle.m_DataChannel), pSetting->m_SendOption);
    result = ConvertUdsSendToResult(result);
    m_HandleCriticalSection.Unlock();
    return result;
}

// 0x00416FD4
nn::Result nn::pia::local::UdsNetworkManager::EjectClient(const nn::pia::common::StationAddress& address)
{
    nn::Result result = nn::uds::CTR::EjectClient(ConvertTransportIdToLocalNodeId(static_cast<u8>(address.m_ExtensionId)));
    return ConvertNetworkResult(this, result);
}

// 0x0041701C
void nn::pia::local::UdsNetworkManager::SetupParams()
{
    m_InvalidNodeId = 0;
    m_BroadcastNodeId = NODE_ID_BROADCAST;
    m_ApplicationDataSize = BEACON_DATA_SIZE;
}

// 0x00417040
nn::Result nn::pia::local::UdsNetworkManager::StartupImpl()
{
    nn::Result result = nn::uds::CTR::Initialize(&m_StatusEvent, LocalNetwork::s_pInstance->m_pReceiveBuffer, LocalNetwork::s_pInstance->m_ReceiveBufferSize, m_pUserName);
    if (result.IsSuccess()) {
        return nn::Result();
    }
    if (result == nn::uds::CTR::RESULT_STATUS_CHANGED_2) {
        return common::RESULT_LOCAL_NETWORK_LOST;
    }
    return common::RESULT_INVALID_STATE;
}

// 0x0041709C
void nn::pia::local::UdsNetworkManager::UpdateConnectionStatus()
{
    UdsNetworkConnectionStatus status;
    nn::uds::CTR::GetConnectionStatus(&status.m_Status);
    SetConnectionStatus(&status);
}

// 0x004170D8
nn::Result nn::pia::local::UdsNetworkManager::EjectAllClients()
{
    nn::Result result = nn::uds::CTR::EjectClient(NODE_ID_BROADCAST);
    return ConvertNetworkResult(this, result);
}

// 0x00417118
nn::Result nn::pia::local::UdsNetworkManager::CreateSessionId()
{
    u8 source[SESSION_ID_SOURCE_SIZE];
    if (nn::uds::CTR::detail::GetMacAddress(source).IsFailure()) {
        return common::RESULT_LOCAL_NETWORK_UNAVAILABLE;
    }
    pead::Random random;
    for (u32 i = MAC_ADDRESS_SIZE; i < SESSION_ID_SOURCE_SIZE; i++) {
        source[i] = static_cast<u8>(random.getU32());
    }
    SetSessionId(pead::HashCrc32::calcHash(source, SESSION_ID_SOURCE_SIZE));
    return nn::Result();
}

// 0x0041718C
nn::Result nn::pia::local::UdsNetworkManager::ReceiveFromCore(void* pBuffer, u32 bufferSize, u32* pReceivedSize, u16* pNodeId)
{
    m_HandleCriticalSection.Lock();
    if (!m_Handle.m_IsCreated) {
        m_HandleCriticalSection.Unlock();
        return common::RESULT_LOCAL_NETWORK_LOST;
    }
    const UdsNetworkSetting* pSetting = static_cast<const UdsNetworkSetting*>(m_pSetting);
    nn::Result result = nn::uds::CTR::ReceiveFrom(m_Handle.m_ReceiveEndpoint, pBuffer, pReceivedSize, pNodeId, bufferSize, pSetting->m_ReceiveOption);
    result = ConvertUdsReceiveFromResult(result);
    m_HandleCriticalSection.Unlock();
    return result;
}

// 0x00417238
nn::Result nn::pia::local::UdsNetworkManager::AllowParticipating()
{
    nn::Result result = nn::uds::CTR::AllowToConnect();
    return ConvertNetworkResult(this, result);
}

// 0x00417270
nn::Result nn::pia::local::UdsNetworkManager::CreateSystemHandle()
{
    m_HandleCriticalSection.Lock();
    if (!m_Handle.m_IsCreated) {
        nn::Result result = m_Handle.CreateHandle(DATA_CHANNEL);
        if (result.IsFailure()) {
            if (result == nn::uds::CTR::RESULT_STATUS_CHANGED_2) {
                m_HandleCriticalSection.Unlock();
                return common::RESULT_LOCAL_NETWORK_LOST;
            }
            m_HandleCriticalSection.Unlock();
            return common::RESULT_INVALID_STATE;
        }
    }
    m_HandleCriticalSection.Unlock();
    return nn::Result();
}

// 0x0041730C
nn::Result nn::pia::local::UdsNetworkManager::GetStationInfoList(nn::pia::local::LocalStationInfo* pInfos, u8 infoNum, u32 index, const void* pScanBuffer) const
{
    if (!common::IsValidPointer(pInfos) || !common::IsValidPointer(pScanBuffer)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    nn::uds::CTR::ScanResultReader reader(pScanBuffer);
    if (reader.GetCount() == 0) {
        return common::RESULT_INVALID_STATE;
    }
    if (reader.GetCount() <= index) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    const LocalNetworkDescription* pDescription = LocalNetwork::s_pInstance->GetNetworkDescription(index);
    if (!common::IsValidPointer(pDescription) || pDescription->GetMaxParticipants() > infoNum) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    nn::uds::CTR::NodeInformation* pNodeInformations = const_cast<UdsNetworkManager*>(this)->m_NodeInformations;
    for (u32 i = 0; i <= index; i++) {
        nn::uds::CTR::NetworkDescriptionReader descriptionReader = reader.GetNextDescription();
        if (i != index) {
            continue;
        }
        nn::Result result = descriptionReader.GetNodeInformationList(pNodeInformations);
        if (result.IsSuccess() && infoNum != 0) {
            for (u32 j = 0; j < infoNum; j++) {
                pInfos[j].m_Role = LocalStationInfo::ROLE_NONE;
            }
            for (u32 j = 0; j < NODE_INFORMATION_NUM; j++) {
                if (j >= infoNum) {
                    break;
                }
                CopyStationInfo(&pInfos[j], pNodeInformations[j], ConvertLocalNodeIdToStationInfoRole(pNodeInformations[j].nodeId));
            }
        }
        return ConvertUdsResult(result);
    }
    return common::RESULT_NOT_INITIALIZED;
}

// 0x004174D4
void nn::pia::local::UdsNetworkManager::ProcessUpdateEvent()
{
    UdsNetworkConnectionStatus connectionStatus;
    GetConnectionStatus(&connectionStatus);
    const nn::uds::CTR::ConnectionStatus& status = connectionStatus.m_Status;
    u16 changedNodes = status.changedNodes;
    switch (status.status) {
    case STATUS_DESTROYED:
        ClearIds();
        LocalNetwork::s_pInstance->m_DisconnectReason = ConvertDisconnectReason(status.reason);
        break;
    case STATUS_HOST:
    case STATUS_CLIENT:
    case STATUS_SPECTATOR: {
        // the nodes that came and left
        u32 connectedBitmap = 0;
        u32 disconnectedBitmap = 0;
        for (u32 i = 0; i < NODE_NUM_MAX; i++) {
            u32 bit = 1 << i;
            if ((bit & changedNodes) == 0) {
                continue;
            }
            if (status.nodeIds[i] != m_InvalidNodeId) {
                if (m_NodeIds[i] != m_InvalidNodeId) {
                    disconnectedBitmap |= bit;
                }
                connectedBitmap |= bit;
            } else if (m_NodeIds[i] != m_InvalidNodeId) {
                disconnectedBitmap |= bit;
            }
        }
        for (u8 i = 0; i < NODE_NUM_MAX; i++) {
            if ((disconnectedBitmap & (1 << i)) != 0) {
                ProcessDisconnectEvent(status, i);
            }
        }
        for (u8 i = 0; i < NODE_NUM_MAX; i++) {
            if ((connectedBitmap & (1 << i)) != 0) {
                ProcessConnectEvent(status, i);
            }
        }
        if ((connectedBitmap | disconnectedBitmap) == 0 || status.status != STATUS_HOST) {
            break;
        }
        // the host sends the new node table (not while it leaves for the host migration)
        if (LocalNetwork::s_pInstance->IsEnableHostMigration()) {
            LocalMigrationManager* pMigrationManager = LocalNetwork::s_pInstance->m_pMigrationManager;
            if (pMigrationManager->m_State == LocalMigrationManager::MIGRATION_STATE_HOST_LEAVING && !pMigrationManager->m_IsHostLeaving) {
                break;
            }
            SendUpdateSessionMessage();
        } else {
            SendUpdateSessionMessage();
        }
        break;
    }
    case STATUS_DISCONNECTED: {
        LocalNetwork* pNetwork = LocalNetwork::s_pInstance;
        if (pNetwork->m_DisconnectReason == 1) {
            pNetwork->m_DisconnectReason = ConvertDisconnectReason(status.reason);
        }
        m_pSendMessageJob->m_DestinationBitmap = 0;
        if (pNetwork->IsEnableAroundNetworkSearch()) {
            LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->EndSendMessage();
        }
        u8 hostTransportId = TRANSPORT_ID_INVALID;
        if (!LocalNetwork::s_pInstance->IsEnableHostMigration()) {
            m_SessionVersion = 0;
        } else {
            LocalMigrationManager* pMigrationManager = LocalNetwork::s_pInstance->m_pMigrationManager;
            pMigrationManager->m_IsHostLeaving = false;
            // a client whose host left starts the host migration
            if (LocalNetwork::s_pInstance->m_pMigrationManager->m_State == LocalMigrationManager::MIGRATION_STATE_NONE && IsClient() &&
                (status.reason == 3 || status.reason == 4 || status.reason == 5) && !LocalNetwork::s_pInstance->m_Unknown0x99) {
                LocalNetwork::s_pInstance->m_pMigrationManager->StartHostMigration();
            }
            if (LocalNetwork::s_pInstance->m_pMigrationManager->m_State != LocalMigrationManager::MIGRATION_STATE_NEW_HOST) {
                m_SessionVersion = 0;
            }
            if (LocalNetwork::s_pInstance->m_pMigrationManager->m_State != LocalMigrationManager::MIGRATION_STATE_NONE) {
                hostTransportId = LocalNetwork::s_pInstance->m_pMigrationManager->GetNextHostCandidateTransportId();
            }
        }
        ClearIds();
        m_HostTransportId = hostTransportId;
        m_DisconnectedTransportIdBitmap = 0;
        break;
    }
    }
    m_NodeBitmap = status.nodeBitmap;
}

// 0x0041784C
nn::Result nn::pia::local::UdsNetworkManager::DestroySystemHandle()
{
    m_HandleCriticalSection.Lock();
    if (m_Handle.m_IsCreated && m_Handle.DestroyHandle().IsFailure()) {
        m_HandleCriticalSection.Unlock();
        return common::RESULT_INVALID_STATE;
    }
    m_HandleCriticalSection.Unlock();
    return nn::Result();
}

// 0x004178B8
void nn::pia::local::UdsNetworkManager::ProcessConnectEvent(const nn::uds::CTR::ConnectionStatus& status, u32 index)
{
    u16 nodeId = status.nodeIds[index];
    m_NodeIds[index] = nodeId;
    if (nodeId == status.networkNodeId) {
        m_LocalNodeId = nodeId;
        LocalNetwork::s_pInstance->m_DisconnectReason = ConvertDisconnectReason(status.reason);
    }
    if (LocalNetwork::s_pInstance->IsEnableHostMigration()) {
        if (status.status == STATUS_HOST) {
            // the host gives a station the transport id it had in the old network
            LocalMigrationManager* pMigrationManager = LocalNetwork::s_pInstance->m_pMigrationManager;
            bool isFound = false;
            u64 key = pMigrationManager->GetLocalNodeKey(status.nodeIds[index]);
            u8 transportId = pMigrationManager->GetTransportIdForNodeKey(key, status.nodeCountMax, &isFound);
            if (transportId == TRANSPORT_ID_INVALID || (LocalNetwork::s_pInstance->IsDuringHostMigration() && !isFound)) {
                nn::uds::CTR::EjectClient(status.nodeIds[index]);
                ProcessDisconnectEvent(status, index);
                return;
            }
            pMigrationManager->SetTransportIdToNodeIdTable(transportId, status.nodeIds[index]);
            pMigrationManager->SetNodeInfo(transportId, status.nodeIds[index], key);
            if (status.nodeIds[index] == status.networkNodeId) {
                m_LocalTransportId = transportId;
                m_HostTransportId = transportId;
            }
        }
    } else if (status.status == STATUS_HOST && status.nodeIds[index] == status.networkNodeId) {
        u8 transportId = static_cast<u8>(index + 1);
        m_LocalTransportId = transportId;
        m_HostTransportId = transportId;
    }
    if (!LocalNetwork::s_pInstance->IsEnableAroundNetworkSearch()) {
        return;
    }
    if (status.nodeIds[index] == status.networkNodeId) {
        LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->ResetCommandVersion();
        if (status.status != STATUS_HOST) {
            return;
        }
        LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->PrepareNextCommandStatus();
    }
    // the host tells a new station the command of the search
    if (status.status != STATUS_HOST || status.nodeIds[index] == status.networkNodeId || LocalNetwork::s_pInstance->IsDuringHostMigration()) {
        return;
    }
    LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->StartSendCommandMessage(ConvertLocalNodeIdToTransportId(m_NodeIds[index]));
}

// 0x00417B08
void nn::pia::local::UdsNetworkManager::SetConnectionStatus(const nn::pia::local::LocalConnectionStatus* pStatus)
{
    m_ConnectionStatusCriticalSection.Lock();
    static_cast<UdsNetworkConnectionStatus*>(m_pConnectionStatus)->m_Status = static_cast<const UdsNetworkConnectionStatus*>(pStatus)->m_Status;
    m_ConnectionStatusCriticalSection.Unlock();
}

// 0x00417B64
nn::Result nn::pia::local::UdsNetworkManager::DisallowParticipating(bool isSpectatorDisallowed)
{
    nn::Result result = nn::uds::CTR::DisallowToConnect(isSpectatorDisallowed);
    return ConvertNetworkResult(this, result);
}

// 0x00417BA0
void nn::pia::local::UdsNetworkManager::SetSystemDataToBeacon(void* pBeacon)
{
    if (LocalNetwork::s_pInstance->IsEnableHostMigration()) {
        LocalNetwork::s_pInstance->m_pMigrationManager->SetSystemDataToBeacon(pBeacon);
        return;
    }
    LocalBeaconSystemData data;
    data.m_Unknown0x0 = m_Unknown0x1238;
    data.m_SessionId = m_SessionId;
    data.m_ApplicationVersion = GetApplicationVersion();
    std::memcpy(pBeacon, &data, sizeof(data));
}

// 0x00417C40
nn::Result nn::pia::local::UdsNetworkManager::vf_0x10()
{
    return nn::Result();
}

// 0x00417C48
void nn::pia::local::UdsNetworkManager::ProcessDisconnectEvent(const nn::uds::CTR::ConnectionStatus& status, u32 index)
{
    u8 transportId = ConvertLocalNodeIdToTransportId(m_NodeIds[index]);
    if (LocalNetwork::s_pInstance->IsEnableHostMigration() && status.status == STATUS_CLIENT &&
        (LocalNetwork::s_pInstance->m_pMigrationManager->m_IsHostLeaving || LocalNetwork::s_pInstance->IsDuringHostMigration())) {
        // reported with the next node table
        if (LocalNetwork::s_pInstance->m_pMigrationManager->m_IsHostLeaving) {
            m_DisconnectedTransportIdBitmap |= 1 << (transportId - 1);
        }
    } else {
        LocalNetwork::s_pInstance->ProcessUpdateEventDisconnected(transportId);
    }
    m_NodeIds[index] = m_InvalidNodeId;
    if (LocalNetwork::s_pInstance->IsEnableHostMigration() && status.status == STATUS_HOST) {
        LocalMigrationManager* pMigrationManager = LocalNetwork::s_pInstance->m_pMigrationManager;
        pMigrationManager->ClearTransportIdToNodeIdTable(transportId);
        pMigrationManager->ClearNodeInfo(transportId);
    }
    m_pSendMessageJob->EndSendMessage(transportId);
    if (LocalNetwork::s_pInstance->IsEnableAroundNetworkSearch()) {
        LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->EndSendMessage(transportId);
    }
}

// 0x00417D6C
nn::Result nn::pia::local::UdsNetworkManager::MakeBeaconForCreateNetwork(nn::pia::local::LocalCreateNetworkSetting* pSetting)
{
    if (GetBeaconApplicationDataSizeMax() < pSetting->m_ApplicationDataSize) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    // the system data go in front of the application data
    std::memmove(pSetting->m_ApplicationData + GetBeaconSystemDataSize(), pSetting->m_ApplicationData, pSetting->m_ApplicationDataSize);
    SetSystemDataToBeacon(pSetting->m_ApplicationData);
    pSetting->m_ApplicationDataSize += GetBeaconSystemDataSize();
    return nn::Result();
}

// 0x00417E04
nn::Result nn::pia::local::UdsNetworkManager::SetApplicationData(const void* pData, u32 size)
{
    if (!common::IsValidPointer(pData) || GetBeaconApplicationDataSizeMax() < size) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_ApplicationDataCriticalSection.Lock();
    SetSystemDataToBeacon(m_pApplicationData);
    std::memcpy(m_pApplicationData + GetBeaconSystemDataSize(), pData, size);
    nn::Result result = nn::uds::CTR::SetApplicationData(m_pApplicationData, GetBeaconSystemDataSize() + size);
    if (result.IsFailure()) {
        if (result == nn::uds::CTR::RESULT_NOT_AUTHORIZED_STATE) {
            m_ApplicationDataCriticalSection.Unlock();
            return common::RESULT_LOCAL_NETWORK_LOST;
        }
        result = ConvertUdsResult(result);
        m_ApplicationDataCriticalSection.Unlock();
        return result;
    }
    m_ApplicationDataCriticalSection.Unlock();
    return nn::Result();
}

// 0x00417F20
void nn::pia::local::UdsNetworkManager::Finalize()
{
    m_HandleCriticalSection.Lock();
    if (m_Handle.m_IsCreated) {
        m_Handle.DestroyHandle();
    }
    m_HandleCriticalSection.Unlock();
    if (common::IsValidPointer(m_pSystemData)) {
        if (m_pSystemData != nullptr) {
            pead::FreeMemory(m_pSystemData);
        }
        m_pSystemData = nullptr;
    }
    if (common::IsValidPointer(m_pUserName)) {
        if (m_pUserName != nullptr) {
            pead::FreeMemory(m_pUserName);
        }
        m_pUserName = nullptr;
    }
    if (common::IsValidPointer(m_pConnectionStatus)) {
        if (m_pConnectionStatus != nullptr) {
            pead::FreeMemory(m_pConnectionStatus);
        }
        m_pConnectionStatus = nullptr;
    }
    LocalNetworkManager::Finalize();
}

// 0x00417FDC | fefates:bytes [tier B]
nn::pia::local::UdsNetworkManager::UdsNetworkManager() : m_NodeBitmap(0), m_Handle(), m_HandleCriticalSection(-1), m_pUserName(nullptr)
{
    m_InvalidNodeId = 0;
    m_BroadcastNodeId = NODE_ID_BROADCAST;
    m_ApplicationDataSize = BEACON_DATA_SIZE;
    ClearNodeList();
    m_LocalNodeId = m_InvalidNodeId;
}

// 0x00418090
// 0x00418058 (deleting dtor)
nn::pia::local::UdsNetworkManager::~UdsNetworkManager()
{
    // only the members (in the original too)
}

// 0x00468B04
u32 nn::pia::local::UdsNetworkManager::CreateLocalCommunicationId(u32 uniqueId, bool isDemo) const
{
    return nn::uds::CTR::CreateLocalCommunicationId(uniqueId, isDemo);
}

// 0x00469ED4
void nn::pia::local::UdsNetworkManager::CleanupImpl()
{
    m_NodeBitmap = 0;
    ClearNodeList();
    m_LocalNodeId = m_InvalidNodeId;
    m_HandleCriticalSection.Lock();
    if (m_Handle.m_IsCreated) {
        m_Handle.DestroyHandle();
    }
    m_HandleCriticalSection.Unlock();
    nn::uds::CTR::Finalize();
}

// 0x007302B8
u8 nn::pia::local::UdsNetworkManager::GetLinkLevel() const
{
    if (IsHost() || IsClient()) {
        return nn::nwm::CTR::GetWifiLinkLevel();
    }
    return 0;
}

// 0x007302E8
const nn::pia::local::LocalBeaconSystemData* nn::pia::local::UdsNetworkManager::GetSystemData(const nn::pia::local::LocalNetworkDescription* pDescription) const
{
    if (!common::IsValidPointer(pDescription)) {
        return nullptr;
    }
    UdsNetworkManager* pThis = const_cast<UdsNetworkManager*>(this);
    pThis->m_ApplicationDataCriticalSection.Lock();
    u32 size = static_cast<const UdsNetworkDescription*>(pDescription)->m_Description.GetApplicationData(m_pApplicationData, m_ApplicationDataSize);
    if (size < GetBeaconSystemDataSize()) {
        pThis->m_ApplicationDataCriticalSection.Unlock();
        return nullptr;
    }
    std::memcpy(m_pSystemData, m_pApplicationData, GetBeaconSystemDataSize());
    const LocalBeaconSystemData* pSystemData = m_pSystemData;
    pThis->m_ApplicationDataCriticalSection.Unlock();
    return pSystemData;
}

// 0x0073039C
nn::Result nn::pia::local::UdsNetworkManager::GetStationInfo(nn::pia::local::LocalStationInfo* pInfo, const nn::pia::common::StationAddress& address) const
{
    if (!common::IsValidPointer(pInfo)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    u16 nodeId = ConvertTransportIdToLocalNodeId(static_cast<u8>(address.m_ExtensionId));
    if (nodeId == m_InvalidNodeId) {
        return common::RESULT_NOT_FOUND;
    }
    nn::uds::CTR::NodeInformation information;
    nn::Result result = nn::uds::CTR::GetNodeInformation(&information, nodeId);
    if (result.IsSuccess()) {
        CopyStationInfo(pInfo, information, ConvertLocalNodeIdToStationInfoRole(information.nodeId));
    } else if (result == RESULT_UDS_OUT_OF_RANGE_1018) {
        return common::RESULT_NOT_FOUND;
    }
    return ConvertUdsResult(result);
}

// 0x00730480
nn::Result nn::pia::local::UdsNetworkManager::ConvertUdsResult(const nn::Result& result) const
{
    if (result.IsSuccess()) {
        return nn::Result();
    }
    if (result == nn::uds::CTR::RESULT_NOT_INITIALIZED) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (result == nn::uds::CTR::RESULT_NOT_AUTHORIZED_STATE || result == nn::uds::CTR::RESULT_NOT_FOUND_1018 || result == nn::uds::CTR::RESULT_OUT_OF_RESOURCE_1 ||
        result == nn::uds::CTR::RESULT_CANCELED_1019) {
        return common::RESULT_INVALID_STATE;
    }
    if (result == nn::uds::CTR::RESULT_CANCELED_1022) {
        return common::RESULT_TIMEOUT;
    }
    if (result == nn::uds::CTR::RESULT_STATUS_CHANGED_2) {
        return common::RESULT_LOCAL_NETWORK_LOST;
    }
    if (result == nn::uds::CTR::RESULT_OUT_OF_RANGE || result == RESULT_UDS_OUT_OF_RANGE_1018 || result == nn::uds::CTR::RESULT_INVALID_POINTER ||
        result == nn::uds::CTR::RESULT_TOO_LARGE) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (result == nn::uds::CTR::RESULT_BEACON_WITHOUT_NETWORK || result == nn::uds::CTR::RESULT_BEACON_WITHOUT_NODES) {
        return common::RESULT_NO_DATA;
    }
    return common::RESULT_INVALID_STATE;
}

// 0x00730590
nn::Result nn::pia::local::UdsNetworkManager::GetLinkLevel(u8* pLevel, u32 index, const void* pScanBuffer) const
{
    if (!common::IsValidPointer(pLevel) || !common::IsValidPointer(pScanBuffer)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    nn::uds::CTR::ScanResultReader reader(pScanBuffer);
    if (reader.GetCount() == 0) {
        return common::RESULT_INVALID_STATE;
    }
    if (reader.GetCount() <= index) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    for (u32 i = 0; i <= index; i++) {
        nn::uds::CTR::NetworkDescriptionReader descriptionReader = reader.GetNextDescription();
        if (i == index) {
            u8 level = 0;
            nn::Result result = descriptionReader.GetLinkLevel(&level);
            *pLevel = level;
            return ConvertUdsResult(result);
        }
    }
    return common::RESULT_NOT_INITIALIZED;
}

// 0x00730674
u8 nn::pia::local::UdsNetworkManager::GetChannel() const
{
    if (!IsHost() && !IsClient()) {
        return 0;
    }
    u8 channel;
    if (nn::uds::CTR::GetChannel(&channel).IsFailure()) {
        return 0;
    }
    return channel;
}

// 0x007306B4
nn::Result nn::pia::local::UdsNetworkManager::GetApplicationData(void* pBuffer, u32* pSize, u32 bufferSize, const nn::pia::local::LocalNetworkDescription* pDescription) const
{
    if (!common::IsValidPointer(pBuffer) || !common::IsValidPointer(pSize) || !common::IsValidPointer(pDescription)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    UdsNetworkManager* pThis = const_cast<UdsNetworkManager*>(this);
    pThis->m_ApplicationDataCriticalSection.Lock();
    u32 size = static_cast<const UdsNetworkDescription*>(pDescription)->m_Description.GetApplicationData(m_pApplicationData, m_ApplicationDataSize);
    if (GetBeaconSystemDataSize() >= size) {
        *pSize = 0;
        pThis->m_ApplicationDataCriticalSection.Unlock();
        return common::RESULT_NO_DATA;
    }
    size -= GetBeaconSystemDataSize();
    if (bufferSize < size) {
        size = bufferSize;
    }
    RemoveBeaconSystemData(pBuffer, m_pApplicationData, size);
    *pSize = size;
    pThis->m_ApplicationDataCriticalSection.Unlock();
    return nn::Result();
}

// 0x007307A8
void nn::pia::local::UdsNetworkManager::GetConnectionStatus(nn::pia::local::LocalConnectionStatus* pStatus) const
{
    UdsNetworkManager* pThis = const_cast<UdsNetworkManager*>(this);
    pThis->m_ConnectionStatusCriticalSection.Lock();
    static_cast<UdsNetworkConnectionStatus*>(pStatus)->m_Status = static_cast<const UdsNetworkConnectionStatus*>(m_pConnectionStatus)->m_Status;
    pThis->m_ConnectionStatusCriticalSection.Unlock();
}

// 0x00730804
nn::Result nn::pia::local::UdsNetworkManager::ConvertUdsSendToResult(const nn::Result& result) const
{
    if (result.IsSuccess()) {
        return nn::Result();
    }
    if (result == nn::uds::CTR::RESULT_NOT_AUTHORIZED_STATE || result == nn::uds::CTR::RESULT_NOT_INITIALIZED || IsNwmInvalidState(result)) {
        return common::RESULT_INVALID_STATE_103;
    }
    if (result == RESULT_UDS_OUT_OF_RANGE_1018) {
        return common::RESULT_LOCAL_DESTINATION_NOT_FOUND;
    }
    if (result == nn::uds::CTR::RESULT_TOO_LARGE || result == nn::uds::CTR::RESULT_OUT_OF_RANGE) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (result == RESULT_UDS_OUT_OF_RESOURCE_1008) {
        return common::RESULT_BUFFER_IS_FULL;
    }
    if (result == nn::uds::CTR::RESULT_STATUS_CHANGED_2) {
        return common::RESULT_SOCKET_UNAVAILABLE;
    }
    if (result == nn::uds::CTR::RESULT_MISALIGNED_ADDRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    return common::RESULT_INVALID_STATE;
}

// 0x0073091C
nn::Result nn::pia::local::UdsNetworkManager::GetApplicationDataSize(u32* pSize, const nn::pia::local::LocalNetworkDescription* pDescription) const
{
    if (!common::IsValidPointer(pSize) || !common::IsValidPointer(pDescription)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    UdsNetworkManager* pThis = const_cast<UdsNetworkManager*>(this);
    pThis->m_ApplicationDataCriticalSection.Lock();
    u32 size = static_cast<const UdsNetworkDescription*>(pDescription)->m_Description.GetApplicationData(m_pApplicationData, m_ApplicationDataSize);
    if (GetBeaconSystemDataSize() >= size) {
        *pSize = 0;
        pThis->m_ApplicationDataCriticalSection.Unlock();
        return common::RESULT_NO_DATA;
    }
    *pSize = size - GetBeaconSystemDataSize();
    pThis->m_ApplicationDataCriticalSection.Unlock();
    return nn::Result();
}

// 0x007309DC
u32 nn::pia::local::UdsNetworkManager::GetBeaconSystemDataSize() const
{
    if (!LocalNetwork::s_pInstance->IsEnableHostMigration()) {
        return sizeof(LocalBeaconSystemData);
    }
    return LocalNetwork::s_pInstance->m_pMigrationManager->m_SystemDataSize;
}

// 0x00730A0C
nn::Result nn::pia::local::UdsNetworkManager::ConvertUdsReceiveFromResult(const nn::Result& result) const
{
    if (result.IsSuccess()) {
        return nn::Result();
    }
    if (result == nn::uds::CTR::RESULT_NOT_AUTHORIZED_STATE || result == nn::uds::CTR::RESULT_NOT_INITIALIZED || IsNwmInvalidState(result)) {
        return common::RESULT_INVALID_STATE_103;
    }
    if (result == nn::uds::CTR::RESULT_NOT_AUTHORIZED || result == nn::uds::CTR::RESULT_TOO_LARGE || result == nn::uds::CTR::RESULT_OUT_OF_RANGE) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (result == nn::uds::CTR::RESULT_STATUS_CHANGED_2) {
        return common::RESULT_SOCKET_UNAVAILABLE;
    }
    if (result == nn::uds::CTR::RESULT_MISALIGNED_SIZE || result == nn::uds::CTR::RESULT_MISALIGNED_ADDRESS) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    return common::RESULT_INVALID_STATE;
}

// 0x00730B18
nn::Result nn::pia::local::UdsNetworkManager::GetLocalApplicationData(void* pBuffer, u32* pSize, u32 bufferSize) const
{
    if (!common::IsValidPointer(pBuffer) || !common::IsValidPointer(pSize)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    UdsNetworkManager* pThis = const_cast<UdsNetworkManager*>(this);
    pThis->m_ApplicationDataCriticalSection.Lock();
    size_t size = 0;
    nn::Result result = nn::uds::CTR::GetApplicationData(m_pApplicationData, &size, m_ApplicationDataSize);
    if (result.IsFailure() && result != nn::uds::CTR::RESULT_TOO_LARGE) {
        if (result == nn::uds::CTR::RESULT_NOT_AUTHORIZED_STATE) {
            pThis->m_ApplicationDataCriticalSection.Unlock();
            return common::RESULT_NOT_IN_COMMUNICATION;
        }
        result = ConvertUdsResult(result);
        pThis->m_ApplicationDataCriticalSection.Unlock();
        return result;
    }
    // (the beacon without application data is no failure)
    if (result.IsFailure() || GetBeaconSystemDataSize() >= size) {
        *pSize = 0;
        pThis->m_ApplicationDataCriticalSection.Unlock();
        return nn::Result();
    }
    size -= GetBeaconSystemDataSize();
    if (bufferSize < size) {
        pThis->m_ApplicationDataCriticalSection.Unlock();
        return common::RESULT_INVALID_ARGUMENT;
    }
    RemoveBeaconSystemData(pBuffer, m_pApplicationData, size);
    *pSize = size;
    pThis->m_ApplicationDataCriticalSection.Unlock();
    return nn::Result();
}

// 0x00730C98
u32 nn::pia::local::UdsNetworkManager::GetBeaconApplicationDataSizeMax() const
{
    if (LocalNetwork::s_pInstance->IsEnableHostMigration()) {
        return LocalNetwork::s_pInstance->m_pMigrationManager->m_ApplicationDataSizeMax;
    }
    return BEACON_DATA_SIZE - GetBeaconSystemDataSize();
}

// 0x00730CE0
nn::Result nn::pia::local::UdsNetworkManager::GetLocalApplicationDataSize(u32* pSize) const
{
    if (!common::IsValidPointer(pSize)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    UdsNetworkManager* pThis = const_cast<UdsNetworkManager*>(this);
    pThis->m_ApplicationDataCriticalSection.Lock();
    size_t size = 0;
    nn::Result result = nn::uds::CTR::GetApplicationData(m_pApplicationData, &size, m_ApplicationDataSize);
    if (result.IsFailure() && result != nn::uds::CTR::RESULT_TOO_LARGE) {
        if (result == nn::uds::CTR::RESULT_NOT_AUTHORIZED_STATE) {
            pThis->m_ApplicationDataCriticalSection.Unlock();
            return common::RESULT_NOT_IN_COMMUNICATION;
        }
        result = ConvertUdsResult(result);
        pThis->m_ApplicationDataCriticalSection.Unlock();
        return result;
    }
    if (result.IsFailure() || GetBeaconSystemDataSize() >= size) {
        *pSize = 0;
        pThis->m_ApplicationDataCriticalSection.Unlock();
        return nn::Result();
    }
    *pSize = size - GetBeaconSystemDataSize();
    pThis->m_ApplicationDataCriticalSection.Unlock();
    return nn::Result();
}

// 0x00730E14
bool nn::pia::local::UdsNetworkManager::IsDisconnectedByRequestFromSystem() const
{
    UdsNetworkConnectionStatus status;
    GetConnectionStatus(&status);
    return status.m_Status.reason == 3;
}

// 0x00730E50
u8 nn::pia::local::UdsNetworkManager::ConvertLocalNodeIdToStationInfoRole(u16 nodeId) const
{
    if (nodeId == m_InvalidNodeId) {
        return LocalStationInfo::ROLE_NONE;
    }
    return nodeId == NODE_ID_HOST ? LocalStationInfo::ROLE_HOST : LocalStationInfo::ROLE_CLIENT;
}

// 0x00731180
void nn::pia::local::UdsNetworkManager::vf_0x80()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
