#include "nn/pia/local/local_LocalAroundNetworkSearchManager.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchBackgroundJob.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchJob.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager_LocalAroundNetworkSearchCommandAckMessage.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager_LocalAroundNetworkSearchCommandMessage.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager_LocalAroundNetworkStatusAckMessage.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager_LocalAroundNetworkStatusMessage.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"
#include "pead/peadHeapMgr.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
namespace {
inline LocalNetworkManager* GetNetworkManager()
{
    return LocalNetwork::s_pInstance->m_pNetworkManager;
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

// the statuses are cleared (Cleanup, Finalize and a stop of the search)
inline void ClearStatus(LocalAroundNetworkSearchManager::AroundNetworkStatus& status)
{
    status.m_Version = 0;
    status.m_DestinationBitmap = 0;
    status.m_LifeTime = 0;
    status.m_Key = 0;
}
} // namespace

// 0x00423984 | fefates:bytes
nn::Result nn::pia::local::LocalAroundNetworkSearchManager::Initialize()
{
    if (m_pCallContext == nullptr) {
        m_pCallContext = common::NewObject<common::CallContext>();
    }
    if (m_pSearchJob == nullptr) {
        m_pSearchJob = common::NewObject<LocalAroundNetworkSearchJob>();
    }
    if (m_pBackgroundJob == nullptr) {
        m_pBackgroundJob = CreateLocalAroundNetworkSearchBackgroundJob();
    }
    for (u32 i = 0; i < AROUND_NETWORK_STATUS_NUM; i++) {
        if (m_Statuses[i].m_pInfo == nullptr) {
            m_Statuses[i].m_pInfo = CreateLocalAroundNetworkInfo();
        }
    }
    return nn::Result();
}

// 0x00423A40 | fefates:bytes [tier B]
void nn::pia::local::LocalAroundNetworkSearchManager::EndSendMessage(u8 transportId)
{
    m_CriticalSection.Lock();
    u32 bit = 1 << (transportId - 1);
    m_CommandDestinationBitmap &= ~bit;
    for (u32 i = 0; i < AROUND_NETWORK_STATUS_NUM; i++) {
        m_Statuses[i].m_DestinationBitmap &= ~bit;
    }
    m_CriticalSection.Unlock();
}

// 0x00423AC0 | fefates:bytes [tier B]
void nn::pia::local::LocalAroundNetworkSearchManager::EndSendMessage()
{
    m_CriticalSection.Lock();
    m_CommandDestinationBitmap = 0;
    for (u32 i = 0; i < AROUND_NETWORK_STATUS_NUM; i++) {
        m_Statuses[i].m_DestinationBitmap = 0;
    }
    m_CriticalSection.Unlock();
}

// 0x00423B1C
void nn::pia::local::LocalAroundNetworkSearchManager::ResetCommandVersion()
{
    m_CommandVersion = 0;
}

// 0x00423B28
nn::pia::local::LocalAroundNetworkSearchManager::AroundNetworkStatus::AroundNetworkStatus()
    : m_pInfo(nullptr), m_Version(0), m_DestinationBitmap(0), m_LifeTime(0), m_Key(0)
{
}

// 0x00423B44 | fefates:bytes [tier B]
void nn::pia::local::LocalAroundNetworkSearchManager::StartSendCommandMessage(u8 transportId)
{
    m_CommandDestinationBitmap |= 1 << (transportId - 1);
}

// 0x00423B5C | fefates:bytes [tier B]
void nn::pia::local::LocalAroundNetworkSearchManager::PrepareNextCommandStatus()
{
    m_CommandVersion++;
    m_CommandDestinationBitmap = GetNetworkManager()->GetConnectedTransportIdBitmap(true);
}

// 0x00423B90 | fefates:bytes-fuzzy [tier B]
void nn::pia::local::LocalAroundNetworkSearchManager::SendAroundNetworkStatusMessage()
{
    common::CriticalSection& sendCriticalSection = GetNetworkManager()->m_SystemSendCriticalSection;
    sendCriticalSection.Lock();
    m_CriticalSection.Lock();
    for (u32 i = 0; i < AROUND_NETWORK_STATUS_NUM; i++) {
        AroundNetworkStatus& status = m_Statuses[i];
        if (status.m_DestinationBitmap == 0) {
            continue;
        }
        LocalAroundNetworkStatusMessage message(GetNetworkManager()->m_SystemSendBuffer, LocalNetworkManager::MESSAGE_BUFFER_SIZE, status.m_Version);
        SerializeAroundNetworkStatus(&message, &status);
        if (GetNetworkManager()->SendTo(&message, LocalNetworkManager::TRANSPORT_ID_ALL).IsFailure()) {
            m_CriticalSection.Unlock();
            sendCriticalSection.Unlock();
            return;
        }
    }
    m_CriticalSection.Unlock();
    sendCriticalSection.Unlock();
}

// 0x00423CF0
void nn::pia::local::LocalAroundNetworkSearchManager::ParseAroundNetworkStatusMessage(const nn::pia::local::LocalAroundNetworkSearchManager::Message* pMessage)
{
    LocalAroundNetworkStatusMessage message(const_cast<u8*>(pMessage->m_Data), MESSAGE_DATA_SIZE_MAX, 0);
    if (!message.ParseMessageHeader() || message.GetMessageSize() != pMessage->m_Size) {
        return;
    }
    if (m_IsSearching) {
        DeserializeAroundNetworkStatus(&message);
    }
    u32 version = message.m_Value;
    u8 transportId = GetNetworkManager()->ConvertLocalNodeIdToTransportId(pMessage->m_NodeId);
    common::CriticalSection& sendCriticalSection = GetNetworkManager()->m_SystemSendCriticalSection;
    sendCriticalSection.Lock();
    LocalAroundNetworkStatusAckMessage ackMessage(GetNetworkManager()->m_SystemSendBuffer, LocalNetworkManager::MESSAGE_BUFFER_SIZE, version);
    ackMessage.UpdateMessageHeader();
    GetNetworkManager()->SendTo(&ackMessage, transportId);
    sendCriticalSection.Unlock();
}

// 0x00423E8C
void nn::pia::local::LocalAroundNetworkSearchManager::ParseAroundNetworkStatusAckMessage(const nn::pia::local::LocalAroundNetworkSearchManager::Message* pMessage)
{
    LocalAroundNetworkStatusAckMessage message(const_cast<u8*>(pMessage->m_Data), MESSAGE_DATA_SIZE_MAX, 0);
    if (!message.ParseMessageHeader() || LocalNetwork::s_pInstance->IsHost() || message.GetMessageSize() != pMessage->m_Size) {
        return;
    }
    u8 transportId = GetNetworkManager()->ConvertLocalNodeIdToTransportId(pMessage->m_NodeId);
    m_CriticalSection.Lock();
    for (u32 i = 0; i < AROUND_NETWORK_STATUS_NUM; i++) {
        if (m_Statuses[i].m_Version == message.m_Value) {
            m_Statuses[i].m_DestinationBitmap &= ~(1 << (transportId - 1));
            break;
        }
    }
    m_CriticalSection.Unlock();
}

// 0x00423F9C
void nn::pia::local::LocalAroundNetworkSearchManager::SendStopAroundNetworkSearchMessage()
{
    common::CriticalSection& sendCriticalSection = GetNetworkManager()->m_SystemSendCriticalSection;
    sendCriticalSection.Lock();
    LocalAroundNetworkSearchCommandMessage message(GetNetworkManager()->m_SystemSendBuffer, LocalNetworkManager::MESSAGE_BUFFER_SIZE,
                                                   LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_21, m_CommandVersion);
    message.UpdateMessageHeader();
    GetNetworkManager()->SendTo(&message, LocalNetworkManager::TRANSPORT_ID_ALL);
    sendCriticalSection.Unlock();
}

// 0x00424040
void nn::pia::local::LocalAroundNetworkSearchManager::SendStartAroundNetworkSearchMessage()
{
    common::CriticalSection& sendCriticalSection = GetNetworkManager()->m_SystemSendCriticalSection;
    sendCriticalSection.Lock();
    LocalAroundNetworkSearchCommandMessage message(GetNetworkManager()->m_SystemSendBuffer, LocalNetworkManager::MESSAGE_BUFFER_SIZE,
                                                   LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_20, m_CommandVersion);
    message.UpdateMessageHeader();
    GetNetworkManager()->SendTo(&message, LocalNetworkManager::TRANSPORT_ID_ALL);
    sendCriticalSection.Unlock();
}

// 0x00424130
void nn::pia::local::LocalAroundNetworkSearchManager::ParseAroundNetworkSearchCommandMessage(const nn::pia::local::LocalAroundNetworkSearchManager::Message* pMessage)
{
    bool isStart = pMessage->m_Type == LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_20;
    LocalAroundNetworkSearchCommandMessage message(const_cast<u8*>(pMessage->m_Data), MESSAGE_DATA_SIZE_MAX, LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_20, 0);
    if (!message.ParseMessageHeader() || LocalNetwork::s_pInstance->IsHost() || message.GetMessageSize() != pMessage->m_Size) {
        return;
    }
    // a newer command of the host
    if (static_cast<s32>(message.m_Value - m_CommandVersion) > 0) {
        m_CommandVersion = message.m_Value;
        m_IsSearching = isStart;
        if (!isStart) {
            m_CriticalSection.Lock();
            for (u32 i = 0; i < AROUND_NETWORK_STATUS_NUM; i++) {
                ClearStatus(m_Statuses[i]);
            }
            m_CriticalSection.Unlock();
        }
    }
    u32 version = m_CommandVersion;
    u8 transportId = GetNetworkManager()->ConvertLocalNodeIdToTransportId(pMessage->m_NodeId);
    common::CriticalSection& sendCriticalSection = GetNetworkManager()->m_SystemSendCriticalSection;
    sendCriticalSection.Lock();
    LocalAroundNetworkSearchCommandAckMessage ackMessage(GetNetworkManager()->m_SystemSendBuffer, LocalNetworkManager::MESSAGE_BUFFER_SIZE, version);
    ackMessage.UpdateMessageHeader();
    GetNetworkManager()->SendTo(&ackMessage, transportId);
    sendCriticalSection.Unlock();
}

inline void nn::pia::local::LocalAroundNetworkSearchManager::ParseAroundNetworkSearchCommandAckMessage(const nn::pia::local::LocalAroundNetworkSearchManager::Message* pMessage)
{
    LocalAroundNetworkSearchCommandAckMessage message(const_cast<u8*>(pMessage->m_Data), MESSAGE_DATA_SIZE_MAX, 0);
    if (!message.ParseMessageHeader() || !LocalNetwork::s_pInstance->IsHost() || message.GetMessageSize() != pMessage->m_Size) {
        return;
    }
    u8 transportId = GetNetworkManager()->ConvertLocalNodeIdToTransportId(pMessage->m_NodeId);
    if (m_CommandVersion == message.m_Value) {
        m_CommandDestinationBitmap &= ~(1 << (transportId - 1));
    }
}

// 0x004242EC
nn::Result nn::pia::local::LocalAroundNetworkSearchManager::PushSendMessage(const void* pData, u8 type, u32 size, u16 nodeId)
{
    if (m_SendQueueNum >= MESSAGE_NUM_MAX) {
        return common::RESULT_BUFFER_IS_FULL;
    }
    Message& message = m_SendQueue[m_SendQueueNum];
    std::memcpy(message.m_Data, pData, size);
    message.m_Size = size;
    message.m_Type = type;
    message.m_NodeId = nodeId;
    if (!m_IsSendQueueUpdated) {
        m_IsSendQueueUpdated = true;
    }
    m_SendQueueNum++;
    return nn::Result();
}

// 0x004243CC
void nn::pia::local::LocalAroundNetworkSearchManager::PushReceiveMessage(const void* pData, u8 type, u32 size, u16 nodeId)
{
    if (m_ReceiveQueueNum >= MESSAGE_NUM_MAX) {
        return;
    }
    Message& message = m_ReceiveQueue[m_ReceiveQueueNum];
    std::memcpy(message.m_Data, pData, size);
    message.m_Size = size;
    message.m_Type = type;
    message.m_NodeId = nodeId;
    if (!m_IsReceiveQueueUpdated) {
        m_IsReceiveQueueUpdated = true;
    }
    m_ReceiveQueueNum++;
}

// 0x0042444C
void nn::pia::local::LocalAroundNetworkSearchManager::SendMessages()
{
    if (m_SendQueueCopyNum == 0) {
        return;
    }
    for (u32 i = 0; i < m_SendQueueCopyNum; i++) {
        const Message& message = m_SendQueueCopy[i];
        GetNetworkManager()->SendToCore(message.m_Data, message.m_Size, message.m_NodeId);
    }
    m_SendQueueCopyNum = 0;
}

// 0x004244CC
void nn::pia::local::LocalAroundNetworkSearchManager::ParseMessages()
{
    if (!m_IsReceiveQueueUpdated) {
        return;
    }
    for (u32 i = 0; i < m_ReceiveQueueNum; i++) {
        const Message* pMessage = &m_ReceiveQueue[i];
        switch (pMessage->m_Type) {
        case LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_20:
        case LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_21:
            ParseAroundNetworkSearchCommandMessage(pMessage);
            break;
        case LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_22:
            ParseAroundNetworkStatusMessage(pMessage);
            break;
        case LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_36:
            ParseAroundNetworkSearchCommandAckMessage(pMessage);
            break;
        case LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_38:
            ParseAroundNetworkStatusAckMessage(pMessage);
            break;
        }
    }
    m_ReceiveQueueNum = 0;
    m_IsReceiveQueueUpdated = false;
}

// 0x0042465C
void nn::pia::local::LocalAroundNetworkSearchManager::CopySendQueue()
{
    m_SendQueueCopyNum = m_SendQueueNum;
    for (u32 i = 0; i < m_SendQueueCopyNum; i++) {
        m_SendQueueCopy[i] = m_SendQueue[i];
    }
    m_SendQueueNum = 0;
    m_IsSendQueueUpdated = false;
}

// 0x004246D0 (name is ours)
void nn::pia::local::LocalAroundNetworkSearchManager::Cleanup()
{
    m_IsSearching = false;
    m_Unknown0x28 = 0;
    m_CommandVersion = 0;
    m_CommandDestinationBitmap = 0;
    m_StatusVersion = 0;
    for (u32 i = 0; i < AROUND_NETWORK_STATUS_NUM; i++) {
        ClearStatus(m_Statuses[i]);
    }
    if (common::IsValidPointer(m_pSearchJob)) {
        m_pSearchJob->Cleanup();
        m_pSearchJob->Reset(false);
    }
}

// 0x00424750
nn::Result nn::pia::local::LocalAroundNetworkSearchManager::Startup()
{
    if (m_pSearchJob->IsRunning()) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_pSearchJob->GetState() != common::Job::EXECUTE_STATE_IDLE) {
        m_pSearchJob->Reset(true);
    }
    nn::Result result = m_pSearchJob->Startup(m_pCallContext);
    if (result.IsFailure()) {
        return result;
    }
    m_pSearchJob->Ready(false);
    m_SendQueueNum = 0;
    m_SendQueueCopyNum = 0;
    m_IsSendQueueUpdated = false;
    m_ReceiveQueueNum = 0;
    m_IsReceiveQueueUpdated = false;
    return nn::Result();
}

// 0x004247EC | fefates:bytes
void nn::pia::local::LocalAroundNetworkSearchManager::Finalize()
{
    m_IsSearching = false;
    m_Unknown0x28 = 0;
    m_CommandVersion = 0;
    m_CommandDestinationBitmap = 0;
    m_StatusVersion = 0;
    for (u32 i = 0; i < AROUND_NETWORK_STATUS_NUM; i++) {
        ClearStatus(m_Statuses[i]);
        if (common::IsValidPointer(m_Statuses[i].m_pInfo)) {
            if (m_Statuses[i].m_pInfo != nullptr) {
                pead::FreeMemory(m_Statuses[i].m_pInfo);
            }
            m_Statuses[i].m_pInfo = nullptr;
        }
    }
    DestroyObject(m_pBackgroundJob);
    DestroyObject(m_pSearchJob);
    if (common::IsValidPointer(m_pCallContext)) {
        if (m_pCallContext != nullptr) {
            common::DeleteObject(m_pCallContext);
        }
        m_pCallContext = nullptr;
    }
}

// 0x004248F4
nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkSearchManager()
    : m_Setting(), m_pSearchJob(nullptr), m_pBackgroundJob(nullptr), m_pCallContext(nullptr), m_IsSearching(false), m_Unknown0x28(0), m_CommandVersion(0),
      m_CommandDestinationBitmap(0), m_CriticalSection(-1), m_StatusVersion(0), m_SendQueueNum(0), m_SendQueueCopyNum(0), m_IsSendQueueUpdated(false),
      m_ReceiveQueueNum(0), m_IsReceiveQueueUpdated(false)
{
}

// 0x004249C0 | fefates:bytes
// 0x0042499C (deleting dtor)
nn::pia::local::LocalAroundNetworkSearchManager::~LocalAroundNetworkSearchManager()
{
    // only the members (in the original too)
}

// 0x00731770 | fefates:bytes [tier B]
u32 nn::pia::local::LocalAroundNetworkSearchManager::GetMessageDestBitmap() const
{
    return GetNetworkManager()->GetConnectedTransportIdBitmap(true);
}

// 0x00731788 | fefates:bytes [tier B]
bool nn::pia::local::LocalAroundNetworkSearchManager::IsSendingStatusMessage() const
{
    common::CriticalSection& criticalSection = const_cast<LocalAroundNetworkSearchManager*>(this)->m_CriticalSection;
    criticalSection.Lock();
    for (u32 i = 0; i < AROUND_NETWORK_STATUS_NUM; i++) {
        if (m_Statuses[i].m_DestinationBitmap != 0) {
            criticalSection.Unlock();
            return true;
        }
    }
    criticalSection.Unlock();
    return false;
}

// 0x007317E0
bool nn::pia::local::LocalAroundNetworkSearchManager::IsSendingCommandMessage() const
{
    return m_CommandDestinationBitmap != 0;
}

// 0x007317F0
void nn::pia::local::LocalAroundNetworkSearchManager::vf_0x24()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
