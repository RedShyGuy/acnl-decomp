#include "nn/pia/transport/transport_SendThreadStream.h"
#include "nn/pia/common/common_IPacketOutput.h"
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
// 0x004531AC | fefates:bytes [tier B]
nn::Result nn::pia::transport::SendThreadStream::Initialize(nn::pia::common::IPacketOutput* pOutput, unsigned int packetNum, int priority, unsigned int latencyPacketNum, bool isDropEnabled)
{
    if (common::IsValidPointer(m_pOutput)) {
        return common::RESULT_ALREADY_INITIALIZED;
    }
    if (!common::IsValidPointer(pOutput)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (common::WatermarkManager::s_pInstance != nullptr) {
        common::WatermarkManager::s_pInstance->GetWatermark(0)->SetName("SendThreadStream buffer num");
    }
    nn::Result result = InitializeCore("Pia SendThreadStream", priority, packetNum, latencyPacketNum, isDropEnabled, 0);
    if (result.IsFailure()) {
        return result;
    }
    if (common::IsValidPointer(m_pLatencyEmulator)) {
        m_pLatencyEmulator->m_pOutput = pOutput;
    }
    m_pOutput = pOutput;
    m_PacketNum = 0;
    m_TotalSize = 0;
    return nn::Result();
}

// 0x004532C0 slot 0x04 | fefates:bytes
nn::Result nn::pia::transport::SendThreadStream::ProcessOne()
{
    common::Packet* pPacket;
    // a packet that could not be written before is sent again (its signature is updated only)
    bool isNew;
    if (m_PacketStream.m_Reader.m_Count > 0) {
        pPacket = m_PacketStream.m_Reader.Get(0);
        isNew = false;
    } else {
        pPacket = m_PacketStream.m_Reader.PullOne();
        if (!common::IsValidPointer(pPacket)) {
            return common::RESULT_NO_DATA;
        }
        isNew = true;
    }
    if (common::IsValidPointer(NetworkRttManager::s_pInstance)) {
        NetworkRttManager::s_pInstance->WriteToPacket(pPacket);
    }
    if (common::IsValidPointer(common::SignatureManager::s_pInstance)) {
        if (isNew) {
            unsigned int signatureSize = common::SignatureManager::s_pInstance->AppendSignature(
                pPacket->m_DestinationStationAddress, pPacket, common::Packet::BUFFER_SIZE_MAX, pPacket->m_Size);
            pPacket->AssignPayload(signatureSize);
        } else {
            common::SignatureManager::s_pInstance->UpdateSignature(pPacket->m_DestinationStationAddress, pPacket, pPacket->m_Size);
        }
    }
    m_PacketNum++;
    m_TotalSize += pPacket->m_Size;
    if (isDropPacket()) {
        m_PacketStream.m_Reader.Release();
        return nn::Result();
    }
    nn::Result result;
    if (common::IsValidPointer(m_pLatencyEmulator)) {
        result = m_pLatencyEmulator->Write(*pPacket);
    } else {
        result = m_pOutput->Write(*pPacket);
    }
    if (result.IsSuccess()) {
        m_PacketStream.m_Reader.Release();
    }
    return result;
}

// 0x0045343C | fefates:callseq [tier C]
void nn::pia::transport::SendThreadStream::SetMonitoringData()
{
    u8 waitMSec = static_cast<u32>(m_WaitMSec - 1) < 254 ? static_cast<u8>(m_WaitMSec) : 255;
    common::g_SessionStateMonitoringContent.m_SendThreadWaitMSec = waitMSec;
}

// 0x00453460 | fefates:callseq [tier C]
void nn::pia::transport::SendThreadStream::Finalize()
{
    FinalizeCore();
    m_pOutput = nullptr;
}

// 0x00453478 | fefates:callseq [tier C]
nn::pia::transport::SendThreadStream::SendThreadStream() : m_pOutput(nullptr), m_TotalSize(0)
{
}

// 0x0045BD8C | fefates:callseq [tier C]
nn::pia::transport::SendThreadStream::~SendThreadStream()
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
