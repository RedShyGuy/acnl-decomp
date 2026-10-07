#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_TransportThreadStream.h"

namespace nn {
namespace pia {
namespace common {
class IPacketInput;
}
namespace transport {
// RTTI N2nn3pia9transport19ReceiveThreadStreamE @ 0x008D01F4
// vtable 0x00901EA8 (vptr 0x00901EB0), offset_to_top 0, 2 entries
//
// The thread that reads the packets from the network, checks their signature and puts them into
// the PacketStream. The layout is from the constructor and Initialize; the member names are ours.
class ReceiveThreadStream : public ::nn::pia::transport::TransportThreadStream
{
public:
    ReceiveThreadStream(); // 0x00457F28 | fefates:callseq [tier C]
    ~ReceiveThreadStream(); // 0x00457F54 | fefates:callseq [tier C]

    // the count of the bad signatures
    virtual void ResetMonitoringData(); // 0x00457F14 slot 0x00
    virtual nn::Result ProcessOne(); // 0x00457D38 slot 0x04 | fefates:callseq

    nn::Result Initialize(nn::pia::common::IPacketInput* pInput, unsigned int packetNum, int priority, unsigned int latencyPacketNum, bool isDropEnabled); // 0x00457C1C | fefates:bytes [tier B]
    void Finalize(); // 0x00457EFC | fefates:callseq [tier C]
    void SetMonitoringData(); // 0x00457ED8 | fefates:callseq [tier C]

    common::IPacketInput* m_pInput; // 0x80
    u32 m_PacketNum;                // 0x84
    u64 m_TotalSize;                // 0x88
};
ASSERT_SIZE(ReceiveThreadStream, 0x90);
} // namespace transport
} // namespace pia
} // namespace nn
