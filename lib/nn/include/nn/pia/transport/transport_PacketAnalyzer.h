#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CriticalSection.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/transport/transport_PacketAnalysisData.h"

namespace nn {
namespace pia {
namespace common {
class Packet;
}
namespace transport {
// Counts the packets and messages that pass (PacketHandler has one for sending and one for
// receiving). The layout is from the constructor; the member names are ours.
class PacketAnalyzer
{
public:
    // name: the name in the analysis; headerSize: the size of the lower protocol headers that is
    // added to every packet (parameter names are ours)
    PacketAnalyzer(const char* name, unsigned int headerSize); // 0x0044F1B0 | fefates:bytes [tier B]
    ~PacketAnalyzer(); // 0x0044F228 | fefates:bytes [tier B]

    nn::Result Startup(); // 0x0044F114 | fefates:callseq [tier C]
    void Cleanup(); // 0x0044F100 | fefates:bytes [tier B]

    // counts the packet and its messages; isSend: the packet goes to all its stations (parameter
    // name is ours)
    nn::Result UpdatePacketAnalysisData(const nn::pia::common::Packet& packet, bool isSend); // 0x0044EF30 | fefates:callseq [tier C]
    void ClearPacketAnalysisData(); // 0x0044EF0C | fefates:bytes [tier B]
    nn::Result GetPacketAnalysisData(nn::pia::transport::PacketAnalysisData* pData) const; // 0x00734E28 | fefates:callseq [tier C]

    PacketAnalysisData m_Data;                 // 0x000
    mutable common::CriticalSection m_CriticalSection; // 0x340
    common::Time m_StartTime;                  // 0x350
    bool m_IsStarted;                          // 0x358
    u32 m_HeaderSize;                          // 0x35C
};
ASSERT_OFFSET(PacketAnalyzer, m_StartTime, 0x350);
ASSERT_SIZE(PacketAnalyzer, 0x360);
} // namespace transport
} // namespace pia
} // namespace nn
