#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_TransportThreadStream.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport19ReceiveThreadStreamE @ 0x008D01F4
// vtable 0x00901EA8 (vptr 0x00901EB0), offset_to_top 0, 2 entries
class ReceiveThreadStream : public ::nn::pia::transport::TransportThreadStream
{
public:
    ReceiveThreadStream(); // ctor candidate(s) 0x00457F28 (unverified)
    virtual void vf_0x00(); // 0x00457F14 slot 0x00 | virtual slot, introduced by nn::pia::transport::TransportThreadStream
    virtual void ProcessOne(); // 0x00457D38 slot 0x04 | fefates:callseq
    void Initialize(nn::pia::common::IPacketInput*, unsigned int, int, unsigned int, bool); // 0x00457C1C | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
