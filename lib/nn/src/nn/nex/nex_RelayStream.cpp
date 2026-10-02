#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_RelayStream.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::RelayStream::RelayStream()
{
}

// 0x0035B9A4 slot 0x00 | slot vf_0x00 of nn::nex::Stream
nn::nex::RelayStream::~RelayStream()
{
}

// 0x0035B774 slot 0x08 | fefates:bytes
void nn::nex::RelayStream::ReceiveIncomingPacket(unsigned short, unsigned char, nn::nex::Packet*)
{
}

// 0x0036037C slot 0x0C | slot vf_0x0C of nn::nex::Stream
void nn::nex::RelayStream::DoWork()
{
}

} // namespace nex
} // namespace nn
