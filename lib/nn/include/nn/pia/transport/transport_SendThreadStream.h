#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_TransportThreadStream.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport16SendThreadStreamE @ 0x008D01D0
// vtable 0x00901E64 (vptr 0x00901E6C), offset_to_top 0, 2 entries
class SendThreadStream : public ::nn::pia::transport::TransportThreadStream
{
public:
    SendThreadStream(); // ctor candidate(s) 0x00453478 (unverified)
    virtual void ProcessOne(); // 0x004532C0 slot 0x04 | fefates:bytes
    void Initialize(nn::pia::common::IPacketOutput*, unsigned int, int, unsigned int, bool); // 0x004531AC | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
