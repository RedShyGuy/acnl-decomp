#include "nn/pia/local/local_LocalNetworkManager.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/local/local_LocalAckMessage.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager.h"
#include "nn/pia/local/local_LocalConnectionStatus.h"
#include "nn/pia/local/local_LocalDestroyNetworkMessage.h"
#include "nn/pia/local/local_LocalEventCheckBackgroundJob.h"
#include "nn/pia/local/local_LocalEventJob.h"
#include "nn/pia/local/local_LocalMigrationManager.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkSetting.h"
#include "nn/pia/local/local_LocalParseSystemMessageJob.h"
#include "nn/pia/local/local_LocalReceiveFromJob.h"
#include "nn/pia/local/local_LocalSendMessageJob.h"
#include "nn/pia/local/local_LocalSendSystemMessageBackgroundJob.h"
#include "nn/pia/local/local_LocalStartHostMigrationMessage.h"
#include "nn/pia/local/local_LocalUpdateSessionMessage.h"
#include "nn/svc/svc_Api.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
namespace {
// the data of a message of the stations at most (1500 - LocalMessage::HEADER_SIZE - 22)
const u32 DATA_SIZE_MAX = 1466;
// a system message at most
const u32 SYSTEM_MESSAGE_SIZE_MAX = 1478;
// a received message has a header at least
const u32 RECEIVE_SIZE_MIN = LocalMessage::HEADER_SIZE;
// the stations have sent data within this time
const u32 ACTIVE_INPUT_STREAM_MSEC = 100;

// the system messages of the search of the networks around go to its manager
inline bool IsAroundNetworkSearchMessage(u8 type)
{
    return type == LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_20 || type == LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_21 ||
           type == LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_22 || type == LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_36 ||
           type == LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_38;
}

// a job of the manager is released with its virtual destructor
template <typename T>
inline void DestroyObject(T*& p)
{
    if (common::IsValidPointer(p)) {
        if (p != nullptr) {
            common::DeleteObject(p);
        }
        p = nullptr;
    }
}

// a background job stops its step and waits for the end of the background thread
inline void StopBackgroundJob(common::StepSequenceJob* pJob)
{
    pJob->m_IsCancelRequested = true;
    pJob->WaitForCompletion(2);
}
} // namespace

// 0x004183B4 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalNetworkManager::Initialize(nn::pia::local::LocalNetworkSetting* pSetting)
{
    SetupParams();
    m_pSetting = pSetting;
    if (m_pApplicationData == nullptr) {
        m_pApplicationData = common::NewArray<u8>(m_ApplicationDataSize);
    }
    if (m_pSendSystemMessageBackgroundJob == nullptr) {
        m_pSendSystemMessageBackgroundJob = common::NewObject<LocalSendSystemMessageBackgroundJob>();
    }
    if (m_pSendMessageJob == nullptr) {
        m_pSendMessageJob = common::NewObject<LocalSendMessageJob>();
    }
    if (m_pEventJob == nullptr) {
        m_pEventJob = common::NewObject<LocalEventJob>();
    }
    if (m_pEventCheckBackgroundJob == nullptr) {
        m_pEventCheckBackgroundJob = common::NewObject<LocalEventCheckBackgroundJob>();
    }
    if (m_pReceiveFromJob == nullptr) {
        m_pReceiveFromJob = common::NewObject<LocalReceiveFromJob>();
    }
    if (m_pParseSystemMessageJob == nullptr) {
        m_pParseSystemMessageJob = common::NewObject<LocalParseSystemMessageJob>();
    }
    if (m_pCallContext == nullptr) {
        m_pCallContext = common::NewObject<common::CallContext>();
    }
    return nn::Result();
}

// 0x00418568
nn::Result nn::pia::local::LocalNetworkManager::ReceiveFrom(void* pBuffer, u32, u32* pSize, u8* pTransportId, bool isSystemMessageOnly)
{
    if (m_LocalNodeId == m_InvalidNodeId || GetNodeNum() < 2) {
        return common::RESULT_LOCAL_NETWORK_LOST;
    }
    m_ReceiveCriticalSection.Lock();
    LocalMessage message(m_ReceiveBuffer, MESSAGE_BUFFER_SIZE);
    u32 receivedSize = 0;
    u16 nodeId = 0;
    nn::Result result = ReceiveFromCore(message.m_pBuffer, static_cast<u16>(message.m_BufferSize), &receivedSize, &nodeId);
    if (result.IsFailure()) {
        m_ReceiveCriticalSection.Unlock();
        return result;
    }
    if (receivedSize >= RECEIVE_SIZE_MIN && message.ParseMessageHeader()) {
        u8 type = message.m_Type;
        if (type == MESSAGE_TYPE_DATA) {
            if (!isSystemMessageOnly) {
                u8 transportId = ConvertLocalNodeIdToTransportId(nodeId);
                if (transportId != TRANSPORT_ID_INVALID) {
                    *pSize = message.m_DataSize;
                    *pTransportId = transportId;
                    message.GetData(pBuffer, 0, message.m_DataSize);
                    m_ReceiveCriticalSection.Unlock();
                    return nn::Result();
                }
            }
        } else {
            // the system messages wait in the queue for ParseSystemMessages
            m_ReceiveQueueCriticalSection.Lock();
            if (IsAroundNetworkSearchMessage(type)) {
                if (LocalNetwork::s_pInstance->IsEnableAroundNetworkSearch()) {
                    LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->PushReceiveMessage(message.m_pBuffer, type, receivedSize, nodeId);
                }
            } else if (m_ReceiveQueueNum < SYSTEM_MESSAGE_NUM_MAX) {
                SystemMessage& systemMessage = m_ReceiveQueue[m_ReceiveQueueNum];
                std::memcpy(systemMessage.m_Data, message.m_pBuffer, receivedSize);
                systemMessage.m_Size = receivedSize;
                systemMessage.m_Type = type;
                systemMessage.m_NodeId = nodeId;
                if (!m_IsReceiveQueueUpdated) {
                    m_IsReceiveQueueUpdated = true;
                }
                m_ReceiveQueueNum++;
            }
            m_ReceiveQueueCriticalSection.Unlock();
        }
    }
    m_ReceiveCriticalSection.Unlock();
    return common::RESULT_NO_DATA;
}

// 0x004187FC (name is ours)
void nn::pia::local::LocalNetworkManager::SetSessionId(u32 sessionId)
{
    m_SessionId = sessionId;
}

// 0x00418808 | fefates:bytes [tier B]
void nn::pia::local::LocalNetworkManager::ClearNodeList(u16 nodeId)
{
    if (nodeId == m_InvalidNodeId) {
        return;
    }
    for (u32 i = 0; i < NODE_NUM_MAX; i++) {
        if (m_NodeIds[i] == nodeId) {
            m_NodeIds[i] = m_InvalidNodeId;
            return;
        }
    }
}

// 0x00418844 | fefates:bytes [tier B]
void nn::pia::local::LocalNetworkManager::ClearNodeList()
{
    // (the invalid node id has the same two bytes)
    std::memset(m_NodeIds, static_cast<u8>(m_InvalidNodeId), sizeof(m_NodeIds));
}

// 0x00418858
void nn::pia::local::LocalNetworkManager::RenewUnknown0x123C()
{
    m_Unknown0x1238 = m_Unknown0x123C;
    // the system tick in microseconds (2^32 / 0xF46F6F ticks)
    u64 tick = nn::svc::GetSystemTick();
    m_Unknown0x123C = static_cast<u32>((tick * 0xF46F6F) >> 32);
}

// 0x00418888
nn::Result nn::pia::local::LocalNetworkManager::SendTo(const void* pData, u8 type, u32 size, u8 transportId)
{
    if (size > SYSTEM_MESSAGE_SIZE_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_LocalNodeId == m_InvalidNodeId || GetNodeNum() < 2) {
        return common::RESULT_LOCAL_NETWORK_LOST;
    }
    u16 nodeId = ConvertTransportIdToLocalNodeId(transportId);
    if (nodeId == m_InvalidNodeId) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    return PushSendSystemMessage(pData, type, size, nodeId);
}

// 0x00418984 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalNetworkManager::SendTo(const void* pData, u32 size, u8 transportId)
{
    if (size > DATA_SIZE_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_LocalNodeId == m_InvalidNodeId || GetNodeNum() < 2) {
        return common::RESULT_LOCAL_NETWORK_LOST;
    }
    m_SendCriticalSection.Lock();
    LocalMessage message(m_SendBuffer, MESSAGE_BUFFER_SIZE);
    message.SetData(pData, 0, static_cast<u16>(size));
    u16 nodeId = ConvertTransportIdToLocalNodeId(transportId);
    if (nodeId == m_InvalidNodeId) {
        m_SendCriticalSection.Unlock();
        return common::RESULT_LOCAL_DESTINATION_NOT_FOUND;
    }
    nn::Result result = SendToCore(message.m_pBuffer, message.GetMessageSize(), nodeId);
    m_SendCriticalSection.Unlock();
    return result;
}

// 0x00418B04
nn::Result nn::pia::local::LocalNetworkManager::PushSendSystemMessage(const void* pData, u8 type, u32 size, u16 nodeId)
{
    m_SendQueueCriticalSection.Lock();
    if (IsAroundNetworkSearchMessage(type)) {
        if (LocalNetwork::s_pInstance->IsEnableAroundNetworkSearch()) {
            nn::Result result = LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->PushSendMessage(pData, type, size, nodeId);
            m_SendQueueCriticalSection.Unlock();
            return result;
        }
        m_SendQueueCriticalSection.Unlock();
        return common::RESULT_INVALID_STATE;
    }
    if (m_SendQueueNum >= SYSTEM_MESSAGE_NUM_MAX) {
        m_SendQueueCriticalSection.Unlock();
        return common::RESULT_BUFFER_IS_FULL;
    }
    SystemMessage& systemMessage = m_SendQueue[m_SendQueueNum];
    std::memcpy(systemMessage.m_Data, pData, size);
    systemMessage.m_Size = size;
    systemMessage.m_Type = type;
    systemMessage.m_NodeId = nodeId;
    if (!m_IsSendQueueUpdated) {
        m_IsSendQueueUpdated = true;
    }
    m_SendQueueNum++;
    m_SendQueueCriticalSection.Unlock();
    return nn::Result();
}

// 0x00418C48 | fefates:bytes [tier B]
bool nn::pia::local::LocalNetworkManager::IsActiveLocalInputStream()
{
    m_ReceiveTimeCriticalSection.Lock();
    common::Time now;
    now.SetNow();
    bool isActive = static_cast<u32>((now - m_ReceiveTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick()) < ACTIVE_INPUT_STREAM_MSEC;
    m_ReceiveTimeCriticalSection.Unlock();
    return isActive;
}

// 0x00418CC4
void nn::pia::local::LocalNetworkManager::SendUpdateSessionMessage()
{
    m_SessionVersion++;
    m_SystemSendCriticalSection.Lock();
    LocalUpdateSessionMessage message(m_SystemSendBuffer, MESSAGE_BUFFER_SIZE, m_SessionVersion);
    message.SetData(m_NodeIds, 0, sizeof(m_NodeIds));
    if (LocalNetwork::s_pInstance->IsEnableHostMigration()) {
        // the node ids of the host migration and if the host leaves
        message.SetData(LocalNetwork::s_pInstance->m_pMigrationManager->m_pNodeIds, sizeof(m_NodeIds), sizeof(m_NodeIds));
        bool isHostLeaving = LocalNetwork::s_pInstance->m_pMigrationManager->m_State == LocalMigrationManager::MIGRATION_STATE_HOST_LEAVING;
        message.SetData(&isHostLeaving, sizeof(m_NodeIds) * 2, sizeof(isHostLeaving));
    }
    // every client has to answer
    m_pSendMessageJob->SetMessage(message.m_Type, message.m_pBuffer, message.GetMessageSize(), m_SessionVersion, GetConnectedTransportIdBitmap(true), true);
    m_SystemSendCriticalSection.Unlock();
}

// 0x00418E1C | fefates:bytes [tier B]
void nn::pia::local::LocalNetworkManager::ParseUpdateSessionMessage(const nn::pia::local::LocalNetworkManager::SystemMessage* pMessage)
{
    LocalUpdateSessionMessage message(const_cast<u8*>(pMessage->m_Data), SystemMessage::DATA_SIZE_MAX, 0);
    if (!message.ParseMessageHeader()) {
        return;
    }
    // the host does not take it
    if (m_LocalTransportId != TRANSPORT_ID_INVALID && m_LocalTransportId == m_HostTransportId) {
        return;
    }
    if (message.GetMessageSize() != pMessage->m_Size || !m_IsSessionStarted) {
        return;
    }
    u32 version = message.m_Version;
    if (version <= m_SessionVersion) {
        // a table that is known already: only the answer
        u32 sessionVersion = m_SessionVersion;
        u8 transportId = ConvertLocalNodeIdToTransportId(pMessage->m_NodeId);
        m_SystemSendCriticalSection.Lock();
        LocalAckMessage ackMessage(m_SystemSendBuffer, MESSAGE_BUFFER_SIZE, MESSAGE_TYPE_ACK, sessionVersion);
        ackMessage.UpdateMessageHeader();
        SendTo(ackMessage.m_pBuffer, ackMessage.m_Type, ackMessage.GetMessageSize(), transportId);
        m_SystemSendCriticalSection.Unlock();
        return;
    }
    message.GetData(m_HostNodeIds, 0, sizeof(m_HostNodeIds));
    for (u32 i = 0; i < NODE_NUM_MAX; i++) {
        if (m_NodeIds[i] != m_HostNodeIds[i]) {
            return;
        }
    }
    for (u32 i = 0; i < NODE_NUM_MAX; i++) {
        if ((m_DisconnectedTransportIdBitmap & (1 << i)) != 0) {
            LocalNetwork::s_pInstance->ProcessUpdateEventDisconnected(static_cast<u8>(i + 1));
        }
    }
    m_DisconnectedTransportIdBitmap = 0;
    m_Unknown0x123C = message.m_Unknown0x18;
    LocalNetwork::s_pInstance->m_ParticipationState = message.m_ParticipationState;
    bool isAllNodeInfoSet = true;
    if (LocalNetwork::s_pInstance->IsEnableHostMigration()) {
        u32 offset = sizeof(m_HostNodeIds);
        message.GetData(LocalNetwork::s_pInstance->m_pMigrationManager->m_pNodeIds, offset, sizeof(m_HostNodeIds));
        offset += sizeof(m_HostNodeIds);
        for (u32 i = 0; i < NODE_NUM_MAX; i++) {
            u8 transportId = static_cast<u8>(i + 1);
            u16 nodeId = ConvertTransportIdToLocalNodeId(transportId);
            u64 key = LocalNetwork::s_pInstance->m_pMigrationManager->GetLocalNodeKey(nodeId);
            isAllNodeInfoSet &= LocalNetwork::s_pInstance->m_pMigrationManager->SetNodeInfo(transportId, nodeId, key);
        }
        u8 isHostLeaving;
        message.GetData(&isHostLeaving, offset, sizeof(isHostLeaving));
        LocalNetwork::s_pInstance->m_pMigrationManager->m_IsHostLeaving = isHostLeaving == 1;
    }
    if (m_LocalTransportId == TRANSPORT_ID_INVALID || ConvertLocalNodeIdToTransportId(m_LocalNodeId) != m_LocalTransportId) {
        m_LocalTransportId = ConvertLocalNodeIdToTransportId(m_LocalNodeId);
    }
    if (ConvertLocalNodeIdToTransportId(pMessage->m_NodeId) != m_HostTransportId) {
        m_HostTransportId = ConvertLocalNodeIdToTransportId(pMessage->m_NodeId);
    }
    if (isAllNodeInfoSet) {
        m_SessionVersion = version;
        u8 transportId = ConvertLocalNodeIdToTransportId(pMessage->m_NodeId);
        m_SystemSendCriticalSection.Lock();
        LocalAckMessage ackMessage(m_SystemSendBuffer, MESSAGE_BUFFER_SIZE, MESSAGE_TYPE_ACK, version);
        ackMessage.UpdateMessageHeader();
        SendTo(ackMessage.m_pBuffer, ackMessage.m_Type, ackMessage.GetMessageSize(), transportId);
        m_SystemSendCriticalSection.Unlock();
    }
    m_HostTransportIdBitmap = GetConnectedTransportIdBitmap(false);
}

// 0x00419210
void nn::pia::local::LocalNetworkManager::SendDestroyNetworkMessage()
{
    m_SystemSendCriticalSection.Lock();
    LocalDestroyNetworkMessage message(m_SystemSendBuffer, MESSAGE_BUFFER_SIZE);
    message.UpdateMessageHeader();
    SendTo(message.m_pBuffer, message.m_Type, message.GetMessageSize(), TRANSPORT_ID_ALL);
    m_SystemSendCriticalSection.Unlock();
}

// 0x0041927C
void nn::pia::local::LocalNetworkManager::SendSystemMessages()
{
    if (!m_IsSendQueueUpdated &&
        (!LocalNetwork::s_pInstance->IsEnableAroundNetworkSearch() || !LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->m_IsSendQueueUpdated)) {
        return;
    }
    // the queue is copied, the messages go out without the lock
    m_SendQueueCriticalSection.Lock();
    u32 num = m_SendQueueNum;
    for (u32 i = 0; i < num; i++) {
        m_SendQueueCopy[i] = m_SendQueue[i];
    }
    m_SendQueueNum = 0;
    m_IsSendQueueUpdated = false;
    if (LocalNetwork::s_pInstance->IsEnableAroundNetworkSearch()) {
        LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->CopySendQueue();
    }
    m_SendQueueCriticalSection.Unlock();
    for (u32 i = 0; i < num; i++) {
        LocalNetwork::s_pInstance->m_pNetworkManager->SendToCore(m_SendQueueCopy[i].m_Data, m_SendQueueCopy[i].m_Size, m_SendQueueCopy[i].m_NodeId);
    }
    if (LocalNetwork::s_pInstance->IsEnableAroundNetworkSearch()) {
        LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->SendMessages();
    }
}

// 0x004193D0
void nn::pia::local::LocalNetworkManager::ParseSystemMessages()
{
    if (!m_IsReceiveQueueUpdated &&
        (!LocalNetwork::s_pInstance->IsEnableAroundNetworkSearch() || !LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->m_IsReceiveQueueUpdated)) {
        return;
    }
    m_ReceiveQueueCriticalSection.Lock();
    for (u32 i = 0; i < m_ReceiveQueueNum; i++) {
        SystemMessage* pMessage = &m_ReceiveQueue[i];
        switch (pMessage->m_Type) {
        case MESSAGE_TYPE_UPDATE_SESSION:
            ParseUpdateSessionMessage(pMessage);
            break;
        case MESSAGE_TYPE_DESTROY_NETWORK: {
            // the host ends the network: the client leaves it
            LocalDestroyNetworkMessage message(pMessage->m_Data, SystemMessage::DATA_SIZE_MAX);
            if (!message.ParseMessageHeader()) {
                break;
            }
            if (m_LocalTransportId != TRANSPORT_ID_INVALID && m_LocalTransportId == m_HostTransportId) {
                break;
            }
            if (message.GetMessageSize() != pMessage->m_Size) {
                break;
            }
            if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS || LocalNetwork::s_pInstance->m_IsLeaveRequested ||
                LocalNetwork::s_pInstance->IsAsyncRunning()) {
                break;
            }
            LocalNetwork::s_pInstance->m_DisconnectReason = LocalNetwork::DISCONNECT_REASON_DESTROYED_BY_HOST;
            LocalNetwork::s_pInstance->DisconnectNetwork(m_pCallContext);
            break;
        }
        case MESSAGE_TYPE_START_HOST_MIGRATION: {
            LocalStartHostMigrationMessage message(pMessage->m_Data, SystemMessage::DATA_SIZE_MAX);
            if (!message.ParseMessageHeader()) {
                break;
            }
            if (m_LocalTransportId != TRANSPORT_ID_INVALID && m_LocalTransportId == m_HostTransportId) {
                break;
            }
            if (message.GetMessageSize() != pMessage->m_Size) {
                break;
            }
            if (LocalNetwork::s_pInstance->m_pMigrationManager == nullptr || !LocalNetwork::s_pInstance->IsEnableHostMigration() ||
                LocalNetwork::s_pInstance->IsAsyncRunning()) {
                break;
            }
            LocalNetwork::s_pInstance->m_pMigrationManager->m_IsHostLeaving = false;
            LocalNetwork::s_pInstance->m_pMigrationManager->StartHostMigration();
            break;
        }
        case MESSAGE_TYPE_ACK: {
            LocalAckMessage message(pMessage->m_Data, SystemMessage::DATA_SIZE_MAX, MESSAGE_TYPE_ACK, 0);
            if (!message.ParseMessageHeader()) {
                break;
            }
            if (message.GetMessageSize() != pMessage->m_Size) {
                break;
            }
            m_pSendMessageJob->ReceiveAck(ConvertLocalNodeIdToTransportId(pMessage->m_NodeId), message.m_AckValue);
            break;
        }
        }
    }
    m_ReceiveQueueNum = 0;
    m_IsReceiveQueueUpdated = false;
    if (LocalNetwork::s_pInstance->IsEnableAroundNetworkSearch()) {
        LocalNetwork::s_pInstance->m_pAroundNetworkSearchManager->ParseMessages();
    }
    m_ReceiveQueueCriticalSection.Unlock();
}

// 0x004196E8 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalNetworkManager::SendTo(const nn::pia::local::LocalMessage* pMessage, u8 transportId)
{
    return SendTo(pMessage->m_pBuffer, pMessage->m_Type, pMessage->GetMessageSize(), transportId);
}

// 0x00419710
void nn::pia::local::LocalNetworkManager::SendStartHostMigrationMessage()
{
    m_SystemSendCriticalSection.Lock();
    LocalStartHostMigrationMessage message(m_SystemSendBuffer, MESSAGE_BUFFER_SIZE);
    message.UpdateMessageHeader();
    SendTo(message.m_pBuffer, message.m_Type, message.GetMessageSize(), TRANSPORT_ID_ALL);
    m_SystemSendCriticalSection.Unlock();
}

// 0x0041977C (name is ours)
void nn::pia::local::LocalNetworkManager::Cleanup()
{
    if (common::IsValidPointer(m_pParseSystemMessageJob)) {
        m_pParseSystemMessageJob->Reset(false);
    }
    if (common::IsValidPointer(m_pReceiveFromJob)) {
        StopBackgroundJob(m_pReceiveFromJob);
    }
    if (common::IsValidPointer(m_pEventCheckBackgroundJob)) {
        StopBackgroundJob(m_pEventCheckBackgroundJob);
        m_pEventCheckBackgroundJob->Cleanup();
    }
    if (common::IsValidPointer(m_pEventJob)) {
        m_pEventJob->Reset(false);
    }
    if (common::IsValidPointer(m_pSendMessageJob)) {
        m_pSendMessageJob->Cleanup();
        m_pSendMessageJob->Reset(false);
    }
    if (common::IsValidPointer(m_pSendSystemMessageBackgroundJob)) {
        StopBackgroundJob(m_pSendSystemMessageBackgroundJob);
    }
    CleanupImpl();
    m_SessionVersion = 0;
    m_LocalTransportId = TRANSPORT_ID_INVALID;
    m_HostTransportId = TRANSPORT_ID_INVALID;
    m_ReceiveTime = common::Time();
    m_Unknown0x1278 = true;
}

// 0x004198C8 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalNetworkManager::Startup()
{
    nn::Result result = StartupImpl();
    if (result.IsFailure()) {
        return result;
    }
    ClearNodeList();
    std::memset(m_pApplicationData, 0, m_ApplicationDataSize);
    RenewUnknown0x123C();
    result = m_pSendSystemMessageBackgroundJob->Startup();
    if (result.IsFailure()) {
        return result;
    }
    m_pSendSystemMessageBackgroundJob->Ready(true);
    result = m_pSendMessageJob->Startup();
    if (result.IsFailure()) {
        return result;
    }
    m_pSendMessageJob->Ready(false);
    m_IsEventSignaled = false;
    result = m_pEventJob->Startup();
    if (result.IsFailure()) {
        return result;
    }
    m_pEventJob->Ready(false);
    result = m_pEventCheckBackgroundJob->Startup(&m_StatusEvent);
    if (result.IsFailure()) {
        return result;
    }
    m_pEventCheckBackgroundJob->Ready(true);
    result = m_pReceiveFromJob->Startup();
    if (result.IsFailure()) {
        return result;
    }
    m_pReceiveFromJob->Ready(true);
    result = m_pParseSystemMessageJob->Startup();
    if (result.IsFailure()) {
        return result;
    }
    m_pParseSystemMessageJob->Ready(false);
    m_SendQueueNum = 0;
    m_IsSendQueueUpdated = false;
    m_ReceiveQueueNum = 0;
    m_IsReceiveQueueUpdated = false;
    m_DisconnectedTransportIdBitmap = 0;
    m_IsSessionStarted = false;
    return nn::Result();
}

// 0x00419A4C | fefates:bytes [tier B]
void nn::pia::local::LocalNetworkManager::ClearIds()
{
    m_LocalTransportId = TRANSPORT_ID_INVALID;
    m_HostTransportId = TRANSPORT_ID_INVALID;
    m_LocalNodeId = m_InvalidNodeId;
    ClearNodeList();
    if (LocalNetwork::s_pInstance->IsEnableHostMigration()) {
        LocalNetwork::s_pInstance->m_pMigrationManager->ClearTransportIdToNodeIdTable();
    }
}

// 0x00419AA0
void nn::pia::local::LocalNetworkManager::Finalize()
{
    if (common::IsValidPointer(m_pCallContext)) {
        if (m_pCallContext != nullptr) {
            common::DeleteObject(m_pCallContext);
        }
        m_pCallContext = nullptr;
    }
    DestroyObject(m_pParseSystemMessageJob);
    DestroyObject(m_pReceiveFromJob);
    DestroyObject(m_pEventCheckBackgroundJob);
    DestroyObject(m_pEventJob);
    DestroyObject(m_pSendMessageJob);
    DestroyObject(m_pSendSystemMessageBackgroundJob);
    if (common::IsValidPointer(m_pApplicationData)) {
        if (m_pApplicationData != nullptr) {
            common::DeleteArray(m_pApplicationData);
        }
        m_pApplicationData = nullptr;
    }
}

// 0x00427368
void nn::pia::local::LocalNetworkManager::SetReceiveTime(const nn::pia::common::Time& time)
{
    m_ReceiveTimeCriticalSection.Lock();
    m_ReceiveTime = time;
    m_ReceiveTimeCriticalSection.Unlock();
}

// 0x00419C4C | fefates:bytes [tier B]
nn::pia::local::LocalNetworkManager::LocalNetworkManager()
    : m_StatusEvent(), m_pSetting(nullptr), m_SessionVersion(0), m_LocalTransportId(TRANSPORT_ID_INVALID), m_HostTransportId(TRANSPORT_ID_INVALID),
      m_SystemSendCriticalSection(-1), m_SendCriticalSection(-1), m_ReceiveCriticalSection(-1), m_pSendMessageJob(nullptr), m_ConnectionStatusCriticalSection(-1),
      m_pConnectionStatus(nullptr), m_ApplicationDataCriticalSection(-1), m_pApplicationData(nullptr), m_ApplicationDataSize(0), m_SystemDataCriticalSection(-1),
      m_pSystemData(nullptr), m_Unknown0x1238(0), m_Unknown0x123C(0), m_SessionId(0), m_DisconnectedTransportIdBitmap(0), m_pEventJob(nullptr),
      m_pEventCheckBackgroundJob(nullptr), m_pReceiveFromJob(nullptr), m_pSendSystemMessageBackgroundJob(nullptr), m_pParseSystemMessageJob(nullptr),
      m_pCallContext(nullptr), m_ReceiveTimeCriticalSection(-1), m_ReceiveTime(), m_Unknown0x1278(true), m_HostTransportIdBitmap(0), m_SendQueueCriticalSection(-1),
      m_SendQueueNum(0), m_IsSendQueueUpdated(false), m_ReceiveQueueCriticalSection(-1), m_ReceiveQueueNum(0), m_IsReceiveQueueUpdated(false),
      m_EventCriticalSection(-1), m_IsEventSignaled(false), m_IsSessionStarted(false)
{
}

// 0x00419E88
// 0x00419DEC (deleting dtor)
nn::pia::local::LocalNetworkManager::~LocalNetworkManager()
{
    // only the members (in the original too)
}

// 0x00730EAC | fefates:bytes [tier B]
u8 nn::pia::local::LocalNetworkManager::GetNodeNum() const
{
    u8 num = 0;
    for (u32 i = 0; i < NODE_NUM_MAX; i++) {
        if (m_NodeIds[i] != m_InvalidNodeId) {
            num++;
        }
    }
    return num;
}

// 0x00730EDC | fefates:bytes [tier B]
u8 nn::pia::local::LocalNetworkManager::GetApplicationVersion() const
{
    if (!common::IsValidPointer(m_pSetting)) {
        return 0;
    }
    return m_pSetting->m_ApplicationVersion;
}

// 0x00730EFC (name after LocalMigrationManager's)
void nn::pia::local::LocalNetworkManager::RemoveBeaconSystemData(void* pBuffer, const void* pBeacon, u32 size) const
{
    if (LocalNetwork::s_pInstance->IsEnableHostMigration()) {
        LocalNetwork::s_pInstance->m_pMigrationManager->RemoveBeaconSystemData(pBuffer, pBeacon, size);
        return;
    }
    std::memcpy(pBuffer, static_cast<const u8*>(pBeacon) + GetBeaconSystemDataSize(), size);
}

// 0x00730F70
u8 nn::pia::local::LocalNetworkManager::GetConnectedNodeNum()
{
    m_ConnectionStatusCriticalSection.Lock();
    u8 num = m_pConnectionStatus->GetNodeCount();
    m_ConnectionStatusCriticalSection.Unlock();
    return num;
}

// 0x00730FB4 | fefates:bytes [tier B]
u32 nn::pia::local::LocalNetworkManager::GetConnectedTransportIdBitmap(bool isLocalExcluded) const
{
    u32 bitmap = 0;
    for (u32 i = 0; i < NODE_NUM_MAX; i++) {
        if (m_NodeIds[i] == m_InvalidNodeId) {
            continue;
        }
        u8 transportId = ConvertLocalNodeIdToTransportId(m_NodeIds[i]);
        if (transportId == TRANSPORT_ID_INVALID || (isLocalExcluded && m_LocalTransportId == transportId)) {
            continue;
        }
        bitmap |= 1 << (transportId - 1);
    }
    return bitmap;
}

// 0x00731024 | fefates:bytes [tier B]
u8 nn::pia::local::LocalNetworkManager::ConvertLocalNodeIdToTransportId(u16 nodeId) const
{
    if (nodeId == m_InvalidNodeId) {
        return TRANSPORT_ID_INVALID;
    }
    if (nodeId == m_BroadcastNodeId) {
        return TRANSPORT_ID_ALL;
    }
    if (LocalNetwork::s_pInstance->IsEnableHostMigration()) {
        return LocalNetwork::s_pInstance->m_pMigrationManager->ConvertLocalNodeIdToTransportId(nodeId);
    }
    for (u32 i = 0; i < NODE_NUM_MAX; i++) {
        if (m_NodeIds[i] == nodeId) {
            return static_cast<u8>(i + 1);
        }
    }
    return TRANSPORT_ID_INVALID;
}

// 0x007310A0 | fefates:bytes [tier B]
u16 nn::pia::local::LocalNetworkManager::ConvertTransportIdToLocalNodeId(u8 transportId) const
{
    if (transportId == TRANSPORT_ID_ALL) {
        return m_BroadcastNodeId;
    }
    if (transportId == 0 || transportId > NODE_NUM_MAX) {
        return m_InvalidNodeId;
    }
    if (LocalNetwork::s_pInstance->IsEnableHostMigration()) {
        return LocalNetwork::s_pInstance->m_pMigrationManager->ConvertTransportIdToLocalNodeId(transportId);
    }
    return m_NodeIds[transportId - 1];
}

// 0x00731108 | fefates:bytes [tier B]
bool nn::pia::local::LocalNetworkManager::IsWaitingUpdateSessionAckMessage() const
{
    return m_pSendMessageJob->m_DestinationBitmap != 0;
}

// 0x00731120 | fefates:bytes [tier B]
u32 nn::pia::local::LocalNetworkManager::GetConnectedTransportIdBitmapFromHost(bool isLocalExcluded) const
{
    u32 bitmap = 0;
    for (u32 i = 0; i < NODE_NUM_MAX; i++) {
        u32 bit = 1 << i;
        if ((m_HostTransportIdBitmap & bit) == 0 || (isLocalExcluded && m_LocalTransportId == i + 1)) {
            continue;
        }
        bitmap |= bit;
    }
    return bitmap;
}

// 0x00731184
void nn::pia::local::LocalNetworkManager::vf_0x80()
{
    // empty (in the original too)
}

// 0x00731188 | fefates:bytes [tier B]
bool nn::pia::local::LocalNetworkManager::IsHost() const
{
    return m_LocalTransportId != TRANSPORT_ID_INVALID && m_LocalTransportId == m_HostTransportId;
}

// 0x007311AC | fefates:bytes [tier B]
bool nn::pia::local::LocalNetworkManager::IsClient() const
{
    return m_LocalTransportId != TRANSPORT_ID_INVALID && m_LocalTransportId != m_HostTransportId;
}

} // namespace local
} // namespace pia
} // namespace nn
