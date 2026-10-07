#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
// The round trip time and the packet loss of the stations 0..10 for the analysis (the counters of
// their SequenceIdController at the last two updates). The class, Clear and Print are from the
// fefates symbols; the layout is from ConnectionAnalyzer, the member names are ours.
class ConnectionAnalysisData
{
public:
    static const u32 STATION_NUM = 11;

    struct StationData
    {
        s32 m_Rtt;                 // 0x00
        u32 m_LastReceivedNum;     // 0x04
        u32 m_ReceivedNum;         // 0x08
        u32 m_LastLostNum;         // 0x0C
        u32 m_LostNum;             // 0x10
        bool m_IsValid;            // 0x14
    };

    void Clear(); // 0x0045BDAC | fefates:bytes [tier B]
    void Print(bool printAll) const; // 0x0073658C | fefates:bytes [tier B]

    StationData m_StationData[STATION_NUM]; // 0x000
    s32 m_ElapsedMSec;                      // 0x108, since ConnectionAnalyzer::Startup
};
ASSERT_SIZE(ConnectionAnalysisData::StationData, 0x18);
ASSERT_SIZE(ConnectionAnalysisData, 0x10C);
} // namespace transport
} // namespace pia
} // namespace nn
