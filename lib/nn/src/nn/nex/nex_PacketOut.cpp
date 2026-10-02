#include "nn/nex/nex_Packet.h"
#include "nn/nex/nex_PacketOut.h"

namespace nn {
namespace nex {
// TODO: default ctor added so derived stubs compile - may not exist
nn::nex::PacketOut::PacketOut()
{
}

// 0x003D7884 slot 0x00 | fefates:bytes
nn::nex::PacketOut::~PacketOut()
{
}

// 0x003D7580 | fefates:bytes [tier B]
void nn::nex::PacketOut::Initialize(nn::nex::PRUDPEndPoint*, nn::nex::PacketType, unsigned short, nn::nex::Buffer*, unsigned int, nn::nex::Timeout*)
{
}

// 0x003D7780 | fefates:bytes [tier B]
nn::nex::PacketOut::PacketOut(nn::nex::PRUDPEndPoint*, nn::nex::PacketType, unsigned short, nn::nex::Buffer*, unsigned int)
{
}

} // namespace nex
} // namespace nn
