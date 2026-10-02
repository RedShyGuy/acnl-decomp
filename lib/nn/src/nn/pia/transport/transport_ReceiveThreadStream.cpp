#include "nn/pia/transport/transport_TransportThreadStream.h"
#include "nn/pia/transport/transport_ReceiveThreadStream.h"

namespace nn {
namespace pia {
namespace transport {
// ctor candidate(s) 0x00457F28 (unverified)
nn::pia::transport::ReceiveThreadStream::ReceiveThreadStream()
{
}

// 0x00457F14 slot 0x00 | virtual slot, introduced by nn::pia::transport::TransportThreadStream
void nn::pia::transport::ReceiveThreadStream::vf_0x00()
{
}

// 0x00457D38 slot 0x04 | fefates:callseq
void nn::pia::transport::ReceiveThreadStream::ProcessOne()
{
}

// 0x00457C1C | fefates:bytes [tier B]
void nn::pia::transport::ReceiveThreadStream::Initialize(nn::pia::common::IPacketInput*, unsigned int, int, unsigned int, bool)
{
}

} // namespace transport
} // namespace pia
} // namespace nn
