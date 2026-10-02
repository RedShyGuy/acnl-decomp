#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_HighLevelStream.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x00378308 (unverified)
nn::nex::HighLevelStream::HighLevelStream()
{
}

// 0x0037846C slot 0x00 | slot vf_0x00 of nn::nex::Stream
nn::nex::HighLevelStream::~HighLevelStream()
{
}

// 0x00377D88 slot 0x08 | slot vf_0x08 of nn::nex::Stream
void nn::nex::HighLevelStream::ReceiveIncomingPacket(unsigned short, unsigned char, nn::nex::Packet*)
{
}

// 0x003781D0 slot 0x0C | slot vf_0x0C of nn::nex::Stream
void nn::nex::HighLevelStream::DoWork()
{
}

// 0x00377D84 slot 0x14 | virtual slot, introduced by nn::nex::HighLevelStream
void nn::nex::HighLevelStream::vf_0x14()
{
}

// 0x00377EEC slot 0x18 | virtual slot, introduced by nn::nex::HighLevelStream
void nn::nex::HighLevelStream::vf_0x18()
{
}

} // namespace nex
} // namespace nn
