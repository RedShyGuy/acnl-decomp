#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_ConnectionAnalysisData.h"
#include "nn/pia/transport/transport_PacketAnalysisData.h"

namespace nn {
namespace pia {
namespace transport {
// The analysis of the transport: the sent and the received packets and the connections. The class
// and Print are from the fefates symbols; the layout is from TransportAnalyzer, the member names
// are ours.
class TransportAnalysisData
{
public:
    void Print(bool printAll) const; // 0x00736560 | fefates:bytes [tier B]

    PacketAnalysisData m_SendData;              // 0x000
    PacketAnalysisData m_ReceiveData;           // 0x340
    ConnectionAnalysisData m_ConnectionData;    // 0x680
};
ASSERT_SIZE(TransportAnalysisData, 0x790);
} // namespace transport
} // namespace pia
} // namespace nn
