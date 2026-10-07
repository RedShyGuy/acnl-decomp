#include "nn/pia/transport/transport_ResendingMessageManager.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/transport/transport_PacketHandler.h"
#include "nn/nstd/nstd_String.h"
#include "pead/peadHeapMgr.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0097E470
nn::pia::transport::ResendingMessageManager* nn::pia::transport::ResendingMessageManager::s_pInstance;

namespace {
// the interval of the resending (a static initializer at 0x0079FEC0 sets it; name is ours)
// 0x0097E478
common::TimeSpan s_ResendInterval(common::TimeSpan::GetTicksPerMSec().GetTick() * 500);
} // namespace

// 0x0045C6B0 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ResendingMessageManager::Initialize(unsigned int messageNumMax)
{
    m_MessageNumMax = messageNumMax;
    m_pSendTimes = common::NewArray<s64>(m_MessageNumMax);
    m_pTimeouts = common::NewArray<s64>(m_MessageNumMax);
    m_pAckIds = common::NewArray<u32>(m_MessageNumMax);
    m_pMessages = common::NewArray<u8*>(m_MessageNumMax);
    for (u32 i = 0; i < m_MessageNumMax; i++) {
        u8* pMessage = static_cast<u8*>(::operator new(MESSAGE_SIZE_MAX, common::HeapManager::GetHeap(), 4));
        if (pMessage != nullptr) {
            for (u32 j = 0; j < MESSAGE_SIZE_MAX; j++) {
                pMessage[j] = 0;
            }
        }
        m_pMessages[i] = pMessage;
    }
    m_pMessageSizes = common::NewArray<u32>(m_MessageNumMax);
    m_pStationIndices = common::NewArray<StationIndex>(m_MessageNumMax);
    m_pStationAddresses = new common::StationAddress[m_MessageNumMax];
    ProtocolId* pProtocolIds = static_cast<ProtocolId*>(pead::AllocMemory(m_MessageNumMax * sizeof(ProtocolId), common::HeapManager::GetHeap()));
    if (pProtocolIds != nullptr) {
        for (u32 i = 0; i < m_MessageNumMax; i++) {
            ::new (&pProtocolIds[i]) ProtocolId(ProtocolId::INVALID);
        }
    }
    m_pProtocolIds = pProtocolIds;
    m_pAckIds[0] = 0;
    return nn::Result();
}

// 0x0045C988 | fefates:bytes [tier B]
bool nn::pia::transport::ResendingMessageManager::StopResending(unsigned int ackId)
{
    if (ackId == 0) {
        return false;
    }
    for (u32 i = 0; i < m_MessageNumMax; i++) {
        if (m_pAckIds[i] == ackId) {
            m_pAckIds[i] = 0;
            m_pTimeouts[i] = 0;
            m_pSendTimes[i] = 0;
            return true;
        }
    }
    return false;
}

// 0x0045CA08 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ResendingMessageManager::CreateInstance()
{
    if (s_pInstance != nullptr) {
        return common::RESULT_ALREADY_EXISTS;
    }
    s_pInstance = new ResendingMessageManager;
    return nn::Result();
}

// 0x0045CA80 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ResendingMessageManager::SetSendMessage(unsigned int* pAckId, const unsigned char* pData, unsigned int size, nn::pia::StationIndex stationIndex, const nn::pia::common::StationAddress& address, nn::pia::transport::ProtocolId protocolId, long long timeout)
{
    if (size > m_pPacketHandler->GetPayloadSizeLimit() - sizeof(u32)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    common::Time now;
    now.SetNow();
    if (timeout != 0 && now.m_Tick >= timeout) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    u32 i;
    for (i = 0; m_pAckIds[i] != 0 && i < m_MessageNumMax; i++) {
    }
    if (i >= m_MessageNumMax) {
        return common::RESULT_BUFFER_IS_FULL;
    }
    // the first ack id from the clock, never 0
    if (m_NextAckId == 0) {
        m_NextAckId = static_cast<u32>(now.m_Tick);
        if (m_NextAckId == 0) {
            m_NextAckId = 1;
        }
    }
    nnnstdMemCpy(m_pMessages[i], pData, size);
    common::serializeU32(m_pMessages[i] + size, m_NextAckId);
    m_pMessageSizes[i] = size + sizeof(u32);
    m_pStationIndices[i] = stationIndex;
    m_pStationAddresses[i] = address;
    m_pProtocolIds[i] = protocolId;
    m_pSendTimes[i] = now.m_Tick;
    m_pTimeouts[i] = timeout;
    m_pAckIds[i] = m_NextAckId;
    m_NextAckId++;
    if (m_NextAckId == 0) {
        m_NextAckId = 1;
    }
    *pAckId = m_pAckIds[i];
    return nn::Result();
}

// 0x0045CBEC | fefates:bytes [tier B]
void nn::pia::transport::ResendingMessageManager::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        s_pInstance->Finalize();
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x0045CC30 | fefates:bytes [tier B]
void nn::pia::transport::ResendingMessageManager::Cleanup()
{
    m_pPacketHandler = nullptr;
    m_pAckIds[0] = 0;
}

// 0x0045CC44 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ResendingMessageManager::Startup(nn::pia::transport::PacketHandler* pPacketHandler)
{
    if (!common::IsValidPointer(pPacketHandler)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_pPacketHandler = pPacketHandler;
    return nn::Result();
}

// 0x0045CC68 | fefates:bytes [tier B]
nn::Result nn::pia::transport::ResendingMessageManager::Dispatch()
{
    common::Time now;
    now.SetNow();
    if (!common::IsValidPointer(m_pPacketHandler)) {
        return nn::Result();
    }
    for (u32 i = 0; i < m_MessageNumMax; i++) {
        if (m_pAckIds[i] == 0) {
            continue;
        }
        s64 timeout = m_pTimeouts[i];
        if (timeout != 0 && now.m_Tick >= timeout) {
            m_pAckIds[i] = 0;
            m_pTimeouts[i] = 0;
            m_pSendTimes[i] = 0;
            continue;
        }
        if (now.m_Tick < m_pSendTimes[i]) {
            continue;
        }
        ProtocolMessageWriter* pWriter;
        if (m_pStationIndices[i] <= STATION_INDEX_MAX) {
            pWriter = m_pPacketHandler->AssignByStationIndex(m_pProtocolIds[i], m_pStationIndices[i], m_pMessageSizes[i], false);
        } else {
            pWriter = m_pPacketHandler->AssignByStationAddress(m_pProtocolIds[i], m_pStationAddresses[i], m_pMessageSizes[i], false);
        }
        if (!common::IsValidPointer(pWriter)) {
            continue;
        }
        pWriter->SetPayload(m_pMessages[i]);
        m_pPacketHandler->Commit();
        m_pSendTimes[i] = now.m_Tick + s_ResendInterval.GetTick();
    }
    return nn::Result();
}

// 0x0045CE04 | fefates:bytes [tier B]
void nn::pia::transport::ResendingMessageManager::Finalize()
{
    if (m_pSendTimes == nullptr) {
        return;
    }
    common::DeleteArray(m_pSendTimes);
    if (m_pTimeouts != nullptr) {
        common::DeleteArray(m_pTimeouts);
    }
    if (m_pAckIds != nullptr) {
        common::DeleteArray(m_pAckIds);
    }
    for (u32 i = 0; i < m_MessageNumMax; i++) {
        if (m_pMessages[i] != nullptr) {
            common::DeleteArray(m_pMessages[i]);
        }
    }
    if (m_pMessages != nullptr) {
        common::DeleteArray(m_pMessages);
    }
    if (m_pMessageSizes != nullptr) {
        common::DeleteArray(m_pMessageSizes);
    }
    if (m_pStationIndices != nullptr) {
        common::DeleteArray(m_pStationIndices);
    }
    delete[] m_pStationAddresses;
    if (m_pProtocolIds != nullptr) {
        common::DeleteArray(m_pProtocolIds);
    }
    m_pSendTimes = nullptr;
    m_pTimeouts = nullptr;
    m_pAckIds = nullptr;
    m_pMessages = nullptr;
    m_pMessageSizes = nullptr;
    m_pStationIndices = nullptr;
    m_pStationAddresses = nullptr;
    m_pProtocolIds = nullptr;
}

// 0x007366C0 | fefates:bytes [tier B]
bool nn::pia::transport::ResendingMessageManager::CheckNowResending(unsigned int ackId) const
{
    if (ackId == 0) {
        return false;
    }
    for (u32 i = 0; i < m_MessageNumMax; i++) {
        if (m_pAckIds[i] == ackId) {
            return true;
        }
    }
    return false;
}

// 0x00736750 | fefates:bytes [tier B]
u32 nn::pia::transport::ResendingMessageManager::ExtractAckIdFromMessage(const unsigned char* pData, unsigned int size) const
{
    if (size < sizeof(u32)) {
        return 0;
    }
    return common::deserializeU32(pData + size - sizeof(u32));
}

// 0x0073676C
void nn::pia::transport::ResendingMessageManager::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
