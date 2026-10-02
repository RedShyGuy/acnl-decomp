#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Stream.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::Stream::Stream()
{
}

// 0x003D113C slot 0x00 | slot vf_0x00 of nn::nex::Stream
nn::nex::Stream::~Stream()
{
}

// 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
void nn::nex::Stream::ReceiveIncomingPacket(unsigned short, unsigned char, nn::nex::Packet*)
{
}

// 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
void nn::nex::Stream::DoWork()
{
}

// 0x003D111C slot 0x10 | slot vf_0x10 of nn::nex::Stream
void nn::nex::Stream::IsDuplicateReorderingPacket(nn::nex::Packet*)
{
}

} // namespace nex
} // namespace nn
