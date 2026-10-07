#include "nn/pia/transport/transport_PacketAnalyzer.h"
#include "nn/pia/common/common_Packet.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/transport/transport_ProtocolMessageReader.h"
#include "nn/pia/transport/transport_Transport.h"
#include <string.h>

namespace nn {
namespace pia {
namespace transport {
// 0x0044EF0C | fefates:bytes [tier B]
void nn::pia::transport::PacketAnalyzer::ClearPacketAnalysisData()
{
    m_CriticalSection.Lock();
    m_Data.ClearCounters();
    m_CriticalSection.Unlock();
}

// 0x0044EF30 | fefates:callseq [tier C]
nn::Result nn::pia::transport::PacketAnalyzer::UpdatePacketAnalysisData(const nn::pia::common::Packet& packet, bool isSend)
{
    if (!m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    if (!packet.IsValid()) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_CriticalSection.Lock();
    u32 packetNum = isSend ? packet.GetPacketNumInNetwork() : 1;
    for (u32 offset = 0; offset < packet.m_Size - common::Packet::HEADER_SIZE;) {
        ProtocolMessageReader reader;
        reader.Attach(packet, offset);
        if (!reader.IsValid()) {
            break;
        }
        ProtocolId protocolId = reader.GetProtocolId();
        u32 messageSize = reader.GetMessageSize();
        s32 index = m_Data.GetIndex(protocolId);
        if (index == -1) {
            index = m_Data.AddProtocol(protocolId);
        }
        if (index != -1) {
            PacketAnalysisData::ProtocolData& data = m_Data.m_ProtocolData[index];
            data.m_PacketNum += packetNum;
            data.m_Size += packetNum * messageSize;
            data.m_TotalPacketNum += packetNum;
            data.m_TotalSize += packetNum * messageSize;
        }
        offset += messageSize;
    }
    m_Data.m_PacketNum += packetNum;
    m_Data.m_TotalPacketNum += packetNum;
    m_Data.m_Size += packetNum * (packet.m_Size + m_HeaderSize);
    m_Data.m_TotalSize += (packet.m_Size + m_HeaderSize) * packetNum;
    m_CriticalSection.Unlock();
    return nn::Result();
}

// 0x0044F100 | fefates:bytes [tier B]
void nn::pia::transport::PacketAnalyzer::Cleanup()
{
    if (m_IsStarted) {
        m_IsStarted = false;
    }
}

// 0x0044F114 | fefates:callseq [tier C]
nn::Result nn::pia::transport::PacketAnalyzer::Startup()
{
    if (m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    m_CriticalSection.Lock();
    m_Data.ClearExceptName();
    m_CriticalSection.Unlock();
    common::Time now = Transport::GetCurrentTime();
    m_IsStarted = true;
    m_StartTime = now;
    return nn::Result();
}

// 0x0044F1B0 | fefates:bytes [tier B]
nn::pia::transport::PacketAnalyzer::PacketAnalyzer(const char* name, unsigned int headerSize)
    : m_Data(), m_CriticalSection(-1), m_IsStarted(false), m_HeaderSize(headerSize)
{
    strncpy(m_Data.m_Name, name, PacketAnalysisData::NAME_LENGTH - 1);
    m_Data.m_Name[PacketAnalysisData::NAME_LENGTH - 1] = '\0';
}

// 0x0044F228 | fefates:bytes [tier B]
nn::pia::transport::PacketAnalyzer::~PacketAnalyzer()
{
    // empty (in the original too: the CriticalSection destructor is trivial)
}

// 0x00734E28 | fefates:callseq [tier C]
nn::Result nn::pia::transport::PacketAnalyzer::GetPacketAnalysisData(nn::pia::transport::PacketAnalysisData* pData) const
{
    if (!common::IsValidPointer(pData)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_CriticalSection.Lock();
    *pData = m_Data;
    m_CriticalSection.Unlock();
    common::Time now = Transport::GetCurrentTime();
    pData->m_ElapsedMSec = (now - m_StartTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick();
    return nn::Result();
}

} // namespace transport
} // namespace pia
} // namespace nn
