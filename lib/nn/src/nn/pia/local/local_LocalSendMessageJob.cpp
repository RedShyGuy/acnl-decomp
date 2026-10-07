#include "nn/pia/local/local_LocalSendMessageJob.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
// 0x0041A404 | fefates:bytes [tier B]
void nn::pia::local::LocalSendMessageJob::ReceiveAck(u8 transportId, u32 ackValue)
{
    if (m_AckValue != ackValue || transportId == LocalNetworkManager::TRANSPORT_ID_INVALID) {
        return;
    }
    m_DestinationBitmap &= ~(1 << (transportId - 1));
}

// 0x0041A430
nn::pia::common::ExecuteResult nn::pia::local::LocalSendMessageJob::SendMessage()
{
    if (m_DestinationBitmap == 0) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    common::Time now;
    now.SetNow();
    if (m_ResendIntervalMSec > static_cast<s32>((now - m_SendTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick())) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    common::Time sendTime;
    sendTime.SetNow();
    m_SendTime = sendTime;
    if (m_IsBroadcast) {
        LocalNetwork::s_pInstance->m_pNetworkManager->SendTo(m_pBuffer, m_Type, m_Size, LocalNetworkManager::TRANSPORT_ID_ALL);
    } else {
        for (u32 i = 0; i < LocalNetworkManager::NODE_NUM_MAX; i++) {
            if ((m_DestinationBitmap & (1 << i)) != 0) {
                LocalNetwork::s_pInstance->m_pNetworkManager->SendTo(m_pBuffer, m_Type, m_Size, i + 1);
            }
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, m_ResendIntervalMSec);
}

// 0x0041A570 | fefates:bytes [tier B]
void nn::pia::local::LocalSendMessageJob::EndSendMessage(u8 transportId)
{
    if (transportId == LocalNetworkManager::TRANSPORT_ID_INVALID) {
        return;
    }
    m_DestinationBitmap &= ~(1 << (transportId - 1));
}

// 0x0041A590
void nn::pia::local::LocalSendMessageJob::Cleanup()
{
    m_DestinationBitmap = 0;
    m_IsBroadcast = false;
    m_AckValue = 0;
    m_Size = 0;
    m_SendTime = common::Time();
}

// 0x0041A5B4 | fefates:bytes [tier B]
nn::Result nn::pia::local::LocalSendMessageJob::Startup()
{
    SetStep(&LocalSendMessageJob::SendMessage, "LocalSendMessageJob::SendMessage");
    return nn::Result();
}

// 0x0041A5F8
nn::Result nn::pia::local::LocalSendMessageJob::SetMessage(u8 type, const void* pData, u32 size, u32 ackValue, u32 destinationBitmap, bool isBroadcast)
{
    if (size > BUFFER_SIZE) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    std::memcpy(m_pBuffer, pData, size);
    m_Type = type;
    m_AckValue = ackValue;
    m_DestinationBitmap = destinationBitmap;
    m_IsBroadcast = isBroadcast;
    m_Size = size;
    return nn::Result();
}

// 0x0041A660 | fefates:bytes [tier B]
nn::pia::local::LocalSendMessageJob::LocalSendMessageJob()
    : m_DestinationBitmap(0), m_IsBroadcast(false), m_AckValue(0), m_CriticalSection(-1), m_Type(1), m_Size(0), m_SendTime(),
      m_ResendIntervalMSec(RESEND_INTERVAL_MSEC)
{
    m_pBuffer = common::NewArray<u32>(BUFFER_SIZE / sizeof(u32));
}

// 0x0041A750
// 0x0041A6F8 (deleting dtor)
nn::pia::local::LocalSendMessageJob::~LocalSendMessageJob()
{
    if (m_pBuffer != nullptr) {
        common::DeleteArray(m_pBuffer);
        m_pBuffer = nullptr;
    }
}

// 0x007311D0
void nn::pia::local::LocalSendMessageJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
