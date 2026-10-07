#include "nn/pia/transport/transport_PacketAnalysisData.h"
#include "nn/pia/transport/transport_AnalysisPrinter.h"

namespace nn {
namespace pia {
namespace transport {
// 0x00457B28 (name is ours)
s32 nn::pia::transport::PacketAnalysisData::AddProtocol(nn::pia::transport::ProtocolId protocolId)
{
    if (m_ProtocolNum >= PROTOCOL_NUM_MAX) {
        return -1;
    }
    m_ProtocolData[m_ProtocolNum].m_ProtocolId = protocolId;
    return m_ProtocolNum++;
}

// 0x00457B54 | fefates:bytes [tier B]
void nn::pia::transport::PacketAnalysisData::ClearCounters()
{
    for (u32 i = 0; i < m_ProtocolNum; i++) {
        m_ProtocolData[i].m_PacketNum = 0;
        m_ProtocolData[i].m_Size = 0;
    }
    m_ElapsedMSec = 0;
    m_PacketNum = 0;
    m_Size = 0;
}

// 0x00457B94 | fefates:bytes [tier B]
void nn::pia::transport::PacketAnalysisData::ClearExceptName()
{
    for (u32 i = 0; i < PROTOCOL_NUM_MAX; i++) {
        m_ProtocolData[i].m_ProtocolId = ProtocolId::INVALID;
        m_ProtocolData[i].m_PacketNum = 0;
        m_ProtocolData[i].m_Size = 0;
        m_ProtocolData[i].m_TotalPacketNum = 0;
        m_ProtocolData[i].m_TotalSize = 0;
    }
    m_ProtocolNum = 0;
    m_ElapsedMSec = 0;
    m_TotalSize = 0;
    m_PacketNum = 0;
    m_Size = 0;
    m_TotalPacketNum = 0;
}

// 0x00457C08 (name is ours)
nn::pia::transport::PacketAnalysisData::ProtocolData::ProtocolData() : m_ProtocolId(ProtocolId::INVALID)
{
}

// 0x00735C0C | fefates:bytes [tier B]
void nn::pia::transport::PacketAnalysisData::Print(bool printAll) const
{
    AnalysisPrinter::Write("[Analysis] ------ BEGIN(%s, %d.%03d sec. passed) ------", m_Name, m_ElapsedMSec / 1000, m_ElapsedMSec % 1000);
    AnalysisPrinter::Write("[Analysis] ProtocolId, TotalNum, TotalSize, AverageSize, Protocol(port)");
    AnalysisPrinter::Write("[Analysis] 0xffffffff, %8d,  %8d,    %8d, Packet", m_PacketNum, m_Size, m_PacketNum != 0 ? m_Size / m_PacketNum : 0);
    for (u32 i = 0; i < m_ProtocolNum; i++) {
        const ProtocolData& data = m_ProtocolData[i];
        if (!printAll) {
            // the protocols of pia itself
            switch (data.m_ProtocolId.GetType()) {
            case PROTOCOL_TYPE_RELAY:
            case PROTOCOL_TYPE_KEEP_ALIVE:
            case PROTOCOL_TYPE_STATION:
            case PROTOCOL_TYPE_MESH:
            case PROTOCOL_TYPE_SYNC_CLOCK:
            case PROTOCOL_TYPE_NAT:
            case PROTOCOL_TYPE_GATEWAY:
            case PROTOCOL_TYPE_RTT:
            case PROTOCOL_TYPE_SESSION:
            case PROTOCOL_TYPE_FEEDBACK:
            case PROTOCOL_TYPE_RELAY_SERVICE:
                continue;
            }
        }
        const char* name;
        switch (data.m_ProtocolId.GetType()) {
        case PROTOCOL_TYPE_SYNC:
            name = "Sync";
            break;
        case PROTOCOL_TYPE_SYNC_CLOCK:
            name = "SyncClock";
            break;
        case PROTOCOL_TYPE_RELAY:
            name = "Relay";
            break;
        case PROTOCOL_TYPE_KEEP_ALIVE:
            name = "KeepAlive";
            break;
        case PROTOCOL_TYPE_STATION:
            name = "Station";
            break;
        case PROTOCOL_TYPE_MESH:
            name = "Mesh";
            break;
        case PROTOCOL_TYPE_NAT:
            name = "Nat";
            break;
        case PROTOCOL_TYPE_GATEWAY:
            name = "Gateway";
            break;
        case PROTOCOL_TYPE_RTT:
            name = "Rtt";
            break;
        case PROTOCOL_TYPE_SYNC_OLD:
            name = "SyncOld";
            break;
        case PROTOCOL_TYPE_RELIABLE:
            name = "Reliable";
            break;
        case PROTOCOL_TYPE_UNRELIABLE:
            name = "Unreliable";
            break;
        case PROTOCOL_TYPE_ROUNDROBIN_UNRELIABLE:
            name = "RoundrobinUnreliable";
            break;
        case PROTOCOL_TYPE_CLONE:
            name = "Clone";
            break;
        case PROTOCOL_TYPE_VOICE:
            name = "Voice";
            break;
        case PROTOCOL_TYPE_RELIABLE_BROADCAST:
            name = "ReliableBroadcast";
            break;
        case PROTOCOL_TYPE_SESSION:
            name = "Session";
            break;
        case PROTOCOL_TYPE_FEEDBACK:
            name = "Feedback";
            break;
        case PROTOCOL_TYPE_RELAY_SERVICE:
            name = "RelayService";
            break;
        default:
            name = "(UNKNOWN PROTOCOL NAME)";
            break;
        }
        AnalysisPrinter::Write("[Analysis] 0x%08x, %8d,  %8d,    %8d, %s(%d)", data.m_ProtocolId.m_Id, data.m_PacketNum, data.m_Size,
                               data.m_PacketNum != 0 ? data.m_Size / data.m_PacketNum : 0xFFFFFFFF, name, data.m_ProtocolId.GetPort());
    }
    AnalysisPrinter::Write("[Analysis] ---------------------------- END ----------------------------");
}

// 0x00736014 | fefates:bytes [tier B]
s32 nn::pia::transport::PacketAnalysisData::GetIndex(nn::pia::transport::ProtocolId protocolId) const
{
    for (u32 i = 0; i < m_ProtocolNum; i++) {
        if (m_ProtocolData[i].m_ProtocolId == protocolId) {
            return i;
        }
    }
    return -1;
}

} // namespace transport
} // namespace pia
} // namespace nn
