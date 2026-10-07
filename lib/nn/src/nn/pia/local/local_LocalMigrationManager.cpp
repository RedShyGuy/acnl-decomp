#include "nn/pia/local/local_LocalMigrationManager.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalHostMigrationJob.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
// 0x0041CBE4 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalMigrationManager::Initialize()
{
    SetupParams();
    if (m_pBeacon == nullptr) {
        m_pBeacon = common::NewArray<u8>(m_SystemDataSize + m_ApplicationDataSizeMax);
    }
    if (m_pPassphrase == nullptr) {
        m_pPassphrase = common::NewArray<u8>(m_PassphraseSizeMax);
    }
    if (m_pHostMigrationJob == nullptr) {
        m_pHostMigrationJob = common::NewObject<LocalHostMigrationJob>();
    }
    if (m_pNodeInfos == nullptr) {
        m_pNodeInfos = common::NewArray<NodeInfo>(NODE_NUM_MAX);
    }
    if (m_pNodeIds == nullptr) {
        m_pNodeIds = common::NewArray<u16>(NODE_NUM_MAX);
    }
    return nn::Result();
}

// 0x0041CD78 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::ClearNodeInfo(u8 transportId)
{
    if (transportId == 0 || transportId > NODE_NUM_MAX) {
        return;
    }
    NodeInfo& info = m_pNodeInfos[transportId - 1];
    info.m_State = NodeInfo::STATE_NONE;
    info.m_TransportId = LocalNetworkManager::TRANSPORT_ID_INVALID;
    info.m_NodeId = LocalNetwork::s_pInstance->m_pNetworkManager->m_InvalidNodeId;
    info.m_Key = m_InvalidNodeKey;
}

// 0x0041CDF8 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::ClearNodeInfo()
{
    for (u32 i = 0; i < NODE_NUM_MAX; i++) {
        ClearNodeInfo(static_cast<u8>(i + 1));
    }
}

// 0x0041CE8C (name is ours)
bool nn::pia::local::LocalMigrationManager::SetNodeInfo(u8 transportId, u16 nodeId, u64 key)
{
    if (nodeId == LocalNetwork::s_pInstance->m_pNetworkManager->m_InvalidNodeId) {
        ClearNodeInfo(transportId);
        return true;
    }
    if (key == m_InvalidNodeKey) {
        return false;
    }
    NodeInfo& info = m_pNodeInfos[transportId - 1];
    info.m_State = NodeInfo::STATE_CONNECTED;
    info.m_TransportId = transportId;
    info.m_NodeId = nodeId;
    info.m_Key = key;
    return true;
}

// 0x0041CF78
nn::Result nn::pia::local::LocalMigrationManager::StartHostMigration()
{
    if (m_State != MIGRATION_STATE_NONE || m_pHostMigrationJob->IsRunning() || LocalNetwork::s_pInstance->m_IsLeaveRequested) {
        return common::RESULT_INVALID_STATE;
    }
    LocalNetwork::s_pInstance->ProcessUpdateEventMigrationStarted();
    if (m_pHostMigrationJob->GetState() != common::Job::EXECUTE_STATE_IDLE) {
        m_pHostMigrationJob->Reset(true);
    }
    // the old host is gone
    LocalNetworkManager* pManager = LocalNetwork::s_pInstance->m_pNetworkManager;
    u8 hostTransportId = pManager->m_HostTransportId;
    ClearTransportIdToNodeIdTable(hostTransportId);
    ClearNodeInfo(hostTransportId);
    for (u32 i = 0; i < NODE_NUM_MAX; i++) {
        if (m_pNodeInfos[i].m_TransportId != LocalNetworkManager::TRANSPORT_ID_INVALID) {
            m_pNodeInfos[i].m_State = NodeInfo::STATE_MIGRATING;
        }
    }
    LocalNetwork::s_pInstance->m_pNetworkManager->m_IsSessionStarted = false;
    m_MigrationResult = MIGRATION_RESULT_NONE;
    u8 localTransportId = LocalNetwork::s_pInstance->m_pNetworkManager->m_LocalTransportId;
    bool isNextHost = localTransportId != LocalNetworkManager::TRANSPORT_ID_INVALID && GetNextHostCandidateTransportId() == localTransportId;
    nn::Result result = m_pHostMigrationJob->Startup(m_pCallContext, isNextHost);
    if (result.IsFailure()) {
        return result;
    }
    m_pHostMigrationJob->Ready(false);
    return nn::Result();
}

// 0x0041D198 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalMigrationManager::CancelHostMigration()
{
    m_pCallContext->Cancel();
    return nn::Result();
}

// 0x0041D1AC | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::SetTransportIdToNodeIdTable(u8 transportId, u16 nodeId)
{
    if (transportId == 0 || transportId > NODE_NUM_MAX) {
        return;
    }
    m_pNodeIds[transportId - 1] = nodeId;
}

// 0x0041D1D0 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::ClearTransportIdToNodeIdTable(u8 transportId)
{
    if (transportId == 0 || transportId > NODE_NUM_MAX) {
        return;
    }
    m_pNodeIds[transportId - 1] = LocalNetwork::s_pInstance->m_pNetworkManager->m_InvalidNodeId;
}

// 0x0041D208 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::ClearTransportIdToNodeIdTable()
{
    for (u32 transportId = 1; transportId <= NODE_NUM_MAX; transportId++) {
        ClearTransportIdToNodeIdTable(static_cast<u8>(transportId));
    }
}

// 0x0041D258 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::Cleanup()
{
    if (common::IsValidPointer(m_pHostMigrationJob)) {
        m_pHostMigrationJob->Cleanup();
        m_pHostMigrationJob->Reset(false);
    }
    m_State = MIGRATION_STATE_NONE;
    m_MigrationResult = MIGRATION_RESULT_NONE;
    m_PassphraseSize = 0;
    m_IsHostLeaving = false;
}

// 0x0041D2A8 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalMigrationManager::Startup()
{
    std::memset(m_pBeacon, 0, m_SystemDataSize + m_ApplicationDataSizeMax);
    std::memset(m_pPassphrase, 0, m_PassphraseSizeMax);
    ClearNodeInfo();
    ClearTransportIdToNodeIdTable();
    return nn::Result();
}

// 0x0041D3A0 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::Finalize()
{
    if (m_pBeacon != nullptr) {
        common::DeleteArray(m_pBeacon);
        m_pBeacon = nullptr;
    }
    if (m_pPassphrase != nullptr) {
        common::DeleteArray(m_pPassphrase);
        m_pPassphrase = nullptr;
    }
    if (common::IsValidPointer(m_pHostMigrationJob)) {
        if (m_pHostMigrationJob != nullptr) {
            common::DeleteObject(m_pHostMigrationJob);
        }
        m_pHostMigrationJob = nullptr;
    }
    if (m_pNodeInfos != nullptr) {
        common::DeleteArray(m_pNodeInfos);
        m_pNodeInfos = nullptr;
    }
    if (m_pNodeIds != nullptr) {
        common::DeleteArray(m_pNodeIds);
        m_pNodeIds = nullptr;
    }
}

// 0x0041D478 | fefates:bytes [tier B]
nn::pia::local::LocalMigrationManager::LocalMigrationManager()
    : m_pNodeInfos(nullptr), m_pNodeIds(nullptr), m_pHostMigrationJob(nullptr), m_State(MIGRATION_STATE_NONE), m_MigrationResult(MIGRATION_RESULT_NONE),
      m_pNextNetworkDescription(nullptr), m_pBeacon(nullptr), m_ApplicationDataSize(0), m_pPassphrase(nullptr), m_PassphraseSize(0), m_pCreateNetworkSetting(nullptr),
      m_pConnectNetworkSetting(nullptr), m_IsHostLeaving(false)
{
    m_pCallContext = common::NewObject<common::CallContext>();
}

// 0x0041D524 | fefates:bytes
// 0x0041D4E8 (deleting dtor)
nn::pia::local::LocalMigrationManager::~LocalMigrationManager()
{
    if (m_pCallContext != nullptr) {
        common::DeleteObject(m_pCallContext);
    }
}

// 0x00731238 | fefates:bytes [tier B]
u8 nn::pia::local::LocalMigrationManager::GetMigrationState(u8 transportId) const
{
    if (transportId == 0 || transportId > NODE_NUM_MAX) {
        return NodeInfo::STATE_NONE;
    }
    return m_pNodeInfos[transportId - 1].m_State;
}

// 0x00731264 | fefates:bytes [tier B]
bool nn::pia::local::LocalMigrationManager::IsExistStateMigrating() const
{
    for (u32 i = 0; i < NODE_NUM_MAX; i++) {
        if (m_pNodeInfos[i].m_State == NodeInfo::STATE_MIGRATING) {
            return true;
        }
    }
    return false;
}

// 0x007312A8 | fefates:bytes [tier B]
void nn::pia::local::LocalMigrationManager::RemoveBeaconSystemData(void* pBuffer, const void* pBeacon, u32 size) const
{
    std::memcpy(pBuffer, static_cast<const u8*>(pBeacon) + m_SystemDataSize, size);
}

// 0x007312C0 (name is ours)
u8 nn::pia::local::LocalMigrationManager::GetTransportIdForNodeKey(u64 key, u32 num, bool* pIsFound) const
{
    *pIsFound = false;
    if (key == m_InvalidNodeKey) {
        return LocalNetworkManager::TRANSPORT_ID_INVALID;
    }
    u32 usedBitmap = 0;
    for (u32 i = 0; i < num; i++) {
        if (m_pNodeInfos[i].m_Key == key) {
            *pIsFound = true;
            return m_pNodeInfos[i].m_TransportId;
        }
        if (m_pNodeInfos[i].m_Key != m_InvalidNodeKey) {
            usedBitmap |= 1 << i;
        }
    }
    for (u32 i = 0; i < num; i++) {
        if ((usedBitmap & (1 << i)) == 0) {
            return static_cast<u8>(i + 1);
        }
    }
    return LocalNetworkManager::TRANSPORT_ID_INVALID;
}

// 0x007313D4 | fefates:bytes [tier B]
u8 nn::pia::local::LocalMigrationManager::ConvertLocalNodeIdToTransportId(u16 nodeId) const
{
    if (nodeId == LocalNetwork::s_pInstance->m_pNetworkManager->m_InvalidNodeId) {
        return LocalNetworkManager::TRANSPORT_ID_INVALID;
    }
    for (u32 i = 0; i < NODE_NUM_MAX; i++) {
        if (m_pNodeIds[i] == nodeId) {
            return static_cast<u8>(i + 1);
        }
    }
    return LocalNetworkManager::TRANSPORT_ID_INVALID;
}

// 0x00731444 | fefates:bytes [tier B]
u16 nn::pia::local::LocalMigrationManager::ConvertTransportIdToLocalNodeId(u8 transportId) const
{
    if (transportId < 1 || transportId > NODE_NUM_MAX) {
        return LocalNetwork::s_pInstance->m_pNetworkManager->m_InvalidNodeId;
    }
    return m_pNodeIds[transportId - 1];
}

// 0x0073147C | fefates:bytes [tier B]
u8 nn::pia::local::LocalMigrationManager::GetNextHostCandidateTransportId() const
{
    u8 hostTransportId = LocalNetwork::s_pInstance->m_pNetworkManager->m_HostTransportId;
    if (hostTransportId == LocalNetworkManager::TRANSPORT_ID_INVALID) {
        return LocalNetworkManager::TRANSPORT_ID_INVALID;
    }
    // the first station after the old host
    u32 bitmap;
    if (LocalNetwork::s_pInstance->IsHost()) {
        bitmap = LocalNetwork::s_pInstance->m_pNetworkManager->GetConnectedTransportIdBitmap(false);
    } else {
        bitmap = LocalNetwork::s_pInstance->m_pNetworkManager->GetConnectedTransportIdBitmapFromHost(false);
    }
    for (u32 i = 0; i < NODE_NUM_MAX; i++) {
        if (i + 1 != hostTransportId && (bitmap & (1 << i)) != 0) {
            return static_cast<u8>(i + 1);
        }
    }
    return LocalNetworkManager::TRANSPORT_ID_INVALID;
}

} // namespace local
} // namespace pia
} // namespace nn
