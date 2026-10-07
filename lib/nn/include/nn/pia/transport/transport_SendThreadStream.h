#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_TransportThreadStream.h"

namespace nn {
namespace pia {
namespace common {
class IPacketOutput;
}
namespace transport {
// RTTI N2nn3pia9transport16SendThreadStreamE @ 0x008D01D0
// vtable 0x00901E64 (vptr 0x00901E6C), offset_to_top 0, 2 entries
//
// The thread that signs the packets of the PacketStream and writes them to the network. The
// layout is from the constructor and Initialize; the member names are ours.
class SendThreadStream : public ::nn::pia::transport::TransportThreadStream
{
public:
    SendThreadStream(); // 0x00453478 | fefates:callseq [tier C]
    ~SendThreadStream(); // 0x0045BD8C | fefates:callseq [tier C]

    virtual nn::Result ProcessOne(); // 0x004532C0 slot 0x04 | fefates:bytes

    nn::Result Initialize(nn::pia::common::IPacketOutput* pOutput, unsigned int packetNum, int priority, unsigned int latencyPacketNum, bool isDropEnabled); // 0x004531AC | fefates:bytes [tier B]
    void Finalize(); // 0x00453460 | fefates:callseq [tier C]
    void SetMonitoringData(); // 0x0045343C | fefates:callseq [tier C]

    common::IPacketOutput* m_pOutput; // 0x80
    u32 m_PacketNum;                  // 0x84
    u64 m_TotalSize;                  // 0x88
};
ASSERT_SIZE(SendThreadStream, 0x90);
} // namespace transport
} // namespace pia
} // namespace nn
