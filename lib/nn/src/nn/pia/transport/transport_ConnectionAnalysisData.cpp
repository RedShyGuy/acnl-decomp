#include "nn/pia/transport/transport_ConnectionAnalysisData.h"
#include "nn/pia/transport/transport_AnalysisPrinter.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0045BDAC | fefates:bytes [tier B]
void nn::pia::transport::ConnectionAnalysisData::Clear()
{
    for (u32 i = 0; i < STATION_NUM; i++) {
        m_StationData[i].m_Rtt = 0;
        m_StationData[i].m_LastReceivedNum = 0;
        m_StationData[i].m_ReceivedNum = 0;
        m_StationData[i].m_LastLostNum = 0;
        m_StationData[i].m_LostNum = 0;
        m_StationData[i].m_IsValid = false;
    }
    m_ElapsedMSec = 0;
}

// 0x0073658C | fefates:bytes [tier B]
void nn::pia::transport::ConnectionAnalysisData::Print(bool) const
{
    AnalysisPrinter::Write("[Analysis] ------ BEGIN(Pia Connection info, %d.%03d sec. passed) ------", m_ElapsedMSec / 1000,
                           m_ElapsedMSec % 1000);
    AnalysisPrinter::Write("[Analysis] StationIndex,  RTT,  PacketLoss");
    for (u32 i = 0; i < STATION_NUM; i++) {
        const StationData& data = m_StationData[i];
        if (!data.m_IsValid) {
            continue;
        }
        u32 receivedNum = data.m_ReceivedNum - data.m_LastReceivedNum;
        u32 lostNum = data.m_LostNum - data.m_LastLostNum;
        // the lost packets in percent (-1: none received)
        f32 packetLoss;
        if (receivedNum == 0) {
            packetLoss = -1.0f;
        } else {
            packetLoss = static_cast<f32>(lostNum) * 100.0f / static_cast<f32>(receivedNum);
        }
        AnalysisPrinter::Write("[Analysis]      0x%02x, %4d,  %6.2f", i, data.m_Rtt, packetLoss);
    }
    AnalysisPrinter::Write("[Analysis] ---------------------------- END ----------------------------");
}

} // namespace transport
} // namespace pia
} // namespace nn
