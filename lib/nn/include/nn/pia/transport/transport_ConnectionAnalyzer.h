#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/transport/transport_ConnectionAnalysisData.h"

namespace nn {
namespace pia {
namespace transport {
// Collects the round trip times and the packet loss of the stations (TransportAnalyzer). The
// class, the constructor, Cleanup and GetConnectionAnalysisData are from the fefates symbols; the
// layout is from the constructor, the member names and the names marked so are ours.
class ConnectionAnalyzer : public ::nn::pia::common::RootObject
{
public:
    ConnectionAnalyzer(); // 0x00457AFC | fefates:bytes [tier B]

    // (name is ours)
    nn::Result Startup(); // 0x00457AB0
    void Cleanup(); // 0x00457A9C | fefates:bytes [tier B]
    // reads the counters of the stations (name is ours)
    nn::Result Update(); // 0x0045794C
    nn::Result GetConnectionAnalysisData(nn::pia::transport::ConnectionAnalysisData* pData) const; // 0x00735B3C | fefates:callseq [tier C]

    ConnectionAnalysisData m_Data; // 0x000
    common::Time m_StartTime;      // 0x110
    bool m_IsStarted;              // 0x118
};
ASSERT_OFFSET(ConnectionAnalyzer, m_StartTime, 0x110);
ASSERT_SIZE(ConnectionAnalyzer, 0x120);
} // namespace transport
} // namespace pia
} // namespace nn
