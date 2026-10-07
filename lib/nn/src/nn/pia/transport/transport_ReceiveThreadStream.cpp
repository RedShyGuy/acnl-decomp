#include "nn/pia/transport/transport_ReceiveThreadStream.h"
#include "nn/pia/common/common_IPacketInput.h"
#include "nn/pia/common/common_Packet.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_SignatureManager.h"
#include "nn/pia/common/common_Watermark.h"
#include "nn/pia/common/common_WatermarkManager.h"
#include "nn/pia/transport/transport_LatencyEmulator.h"
#include "nn/pia/transport/transport_NetworkRttManager.h"

namespace nn {
namespace pia {
namespace transport {
// 0x00457C1C | fefates:bytes [tier B]
nn::Result nn::pia::transport::ReceiveThreadStream::Initialize(nn::pia::common::IPacketInput* pInput, unsigned int packetNum, int priority, unsigned int latencyPacketNum, bool isDropEnabled)
{
    if (common::IsValidPointer(m_pInput)) {
        return common::RESULT_ALREADY_INITIALIZED;
    }
    if (!common::IsValidPointer(pInput)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (common::WatermarkManager::s_pInstance != nullptr) {
        common::WatermarkManager::s_pInstance->GetWatermark(1)->SetName("ReceiveThreadStream buffer num");
    }
    nn::Result result = InitializeCore("Pia ReceiveThreadStream", priority, packetNum, latencyPacketNum, isDropEnabled, 1);
    if (result.IsFailure()) {
        return result;
    }
    if (common::IsValidPointer(m_pLatencyEmulator)) {
        m_pLatencyEmulator->m_pInput = pInput;
    }
    m_pInput = pInput;
    m_PacketNum = 0;
    m_TotalSize = 0;
    return nn::Result();
}

// 0x00457D38 slot 0x04 | fefates:callseq
nn::Result nn::pia::transport::ReceiveThreadStream::ProcessOne()
{
    common::Packet* pPacket;
    if (m_PacketStream.m_Writer.m_Count > 0) {
        pPacket = m_PacketStream.m_Writer.Get(0);
    } else {
        pPacket = m_PacketStream.m_Writer.Assign();
        if (!common::IsValidPointer(pPacket)) {
            return common::RESULT_BUFFER_IS_FULL;
        }
    }
    nn::Result result;
    if (common::IsValidPointer(m_pLatencyEmulator)) {
        result = m_pLatencyEmulator->Read(pPacket);
    } else {
        result = m_pInput->Read(pPacket);
    }
    if (result.IsFailure()) {
        return result;
    }
    if (isDropPacket()) {
        return common::RESULT_NO_DATA;
    }
    m_PacketNum++;
    m_TotalSize += pPacket->m_Size;
    if (!pPacket->IsValid()) {
        return common::RESULT_INVALID_FORMAT;
    }
    if (common::IsValidPointer(common::SignatureManager::s_pInstance)) {
        unsigned int payloadSize;
        if (!common::SignatureManager::s_pInstance->CheckSignature(pPacket->m_SourceStationAddress, pPacket, pPacket->m_Size,
                                                                   &payloadSize)) {
            common::SessionStateMonitoringContent& content = common::g_SessionStateMonitoringContent;
            if (content.m_SignatureErrorNum != 0xFFFFFFFF) {
                content.m_SignatureErrorNum++;
            }
            return common::RESULT_INVALID_FORMAT;
        }
        pPacket->m_Size = payloadSize;
    }
    if (common::IsValidPointer(NetworkRttManager::s_pInstance)) {
        NetworkRttManager::s_pInstance->ReadFromPacket(pPacket);
    }
    m_PacketStream.m_Writer.Push();
    return nn::Result();
}

// 0x00457ED8 | fefates:callseq [tier C]
void nn::pia::transport::ReceiveThreadStream::SetMonitoringData()
{
    u8 waitMSec = static_cast<u32>(m_WaitMSec - 1) < 254 ? static_cast<u8>(m_WaitMSec) : 255;
    common::g_SessionStateMonitoringContent.m_ReceiveThreadWaitMSec = waitMSec;
}

// 0x00457EFC | fefates:callseq [tier C]
void nn::pia::transport::ReceiveThreadStream::Finalize()
{
    FinalizeCore();
    m_pInput = nullptr;
}

// 0x00457F14 slot 0x00
void nn::pia::transport::ReceiveThreadStream::ResetMonitoringData()
{
    common::g_SessionStateMonitoringContent.m_SignatureErrorNum = 0;
}

// 0x00457F28 | fefates:callseq [tier C]
nn::pia::transport::ReceiveThreadStream::ReceiveThreadStream() : m_pInput(nullptr), m_TotalSize(0)
{
}

// 0x00457F54 | fefates:callseq [tier C]
nn::pia::transport::ReceiveThreadStream::~ReceiveThreadStream()
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
