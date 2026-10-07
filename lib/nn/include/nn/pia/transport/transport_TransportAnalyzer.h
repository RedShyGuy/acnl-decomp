#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/transport/transport_TransportAnalysisData.h"

namespace nn {
namespace pia {
namespace transport {
class ConnectionAnalyzer;

// The analysis of the transport (TransportAnalysisData) and its monitoring data. One instance
// (CreateInstance). The class and the functions are from the fefates symbols; the layout is from
// CreateInstance, the member names and the names marked so are ours.
class TransportAnalyzer : public ::nn::pia::common::RootObject
{
public:
    // (inline in CreateInstance)
    TransportAnalyzer() : m_pConnectionAnalyzer(nullptr), m_IsStarted(false) {}
    // (inline in DestroyInstance)
    ~TransportAnalyzer() { Finalize(); }

    static nn::Result CreateInstance(); // 0x004573F0 | fefates:callseq [tier C]
    static void DestroyInstance(); // 0x004574BC | fefates:bytes [tier B]
    // (name is ours)
    nn::Result Initialize(); // 0x0045739C
    void Finalize(); // 0x00457924 | fefates:bytes [tier B]
    nn::Result Startup(); // 0x00457894 | fefates:callseq [tier C]
    // (name is ours)
    void Cleanup(); // 0x0045785C
    // collects the data of the connections and the packets (name is ours)
    nn::Result Update(); // 0x004577A8
    // the packets and the stations into the monitoring data of the session state
    void SetMonitoringData(); // 0x00457514 | fefates:callseq [tier C]

    static TransportAnalyzer* s_pInstance;

    ConnectionAnalyzer* m_pConnectionAnalyzer; // 0x000
    bool m_IsStarted;                          // 0x004
    TransportAnalysisData m_Data;              // 0x008
    common::Time m_StartTime;                  // 0x798
};
ASSERT_OFFSET(TransportAnalyzer, m_StartTime, 0x798);
ASSERT_SIZE(TransportAnalyzer, 0x7A0);
} // namespace transport
} // namespace pia
} // namespace nn
