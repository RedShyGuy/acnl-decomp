#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_RoutingStream.h"

namespace nn {
namespace nex {
// 0x0036D22C slot 0x00 | fefates:callseq
nn::nex::RoutingStream::~RoutingStream()
{
}

// 0x0036C2DC slot 0x08 | fefates:callseq
void nn::nex::RoutingStream::ReceiveIncomingPacket(unsigned short, unsigned char, nn::nex::Packet*)
{
}

// 0x0036D004 slot 0x0C | slot vf_0x0C of nn::nex::Stream
void nn::nex::RoutingStream::DoWork()
{
}

// 0x0036D064 | fefates:bytes-fuzzy [tier B]
nn::nex::RoutingStream::RoutingStream()
{
}

} // namespace nex
} // namespace nn
