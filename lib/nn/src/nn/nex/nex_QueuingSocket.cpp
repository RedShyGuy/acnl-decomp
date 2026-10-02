#include "nn/nex/nex_Socket.h"
#include "nn/nex/nex_QueuingSocket.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x0036A8F0 (unverified)
nn::nex::QueuingSocket::QueuingSocket()
{
}

// 0x0036AA1C slot 0x00 | fefates:callseq
nn::nex::QueuingSocket::~QueuingSocket()
{
}

// 0x0036A7BC slot 0x08 | virtual slot, introduced by nn::nex::Socket
void nn::nex::QueuingSocket::vf_0x08()
{
}

// 0x0036A840 slot 0x0C | virtual slot, introduced by nn::nex::Socket
void nn::nex::QueuingSocket::vf_0x0C()
{
}

// 0x00369624 slot 0x10 | virtual slot, introduced by nn::nex::Socket
void nn::nex::QueuingSocket::vf_0x10()
{
}

// 0x0036A848 slot 0x14 | virtual slot, introduced by nn::nex::Socket
void nn::nex::QueuingSocket::vf_0x14()
{
}

// 0x0072A994 slot 0x18 | virtual slot, introduced by nn::nex::QueuingSocket
void nn::nex::QueuingSocket::vf_0x18()
{
}

// 0x00369594 slot 0x1C | virtual slot, introduced by nn::nex::QueuingSocket
void nn::nex::QueuingSocket::vf_0x1C()
{
}

// 0x00369544 slot 0x20 | virtual slot, introduced by nn::nex::QueuingSocket
void nn::nex::QueuingSocket::vf_0x20()
{
}

// 0x0036A628 | fefates:bytes [tier B]
void nn::nex::QueuingSocket::RetransmitFromHistoryPacketQueue(nn::nex::PacketQueue*, nn::nex::qChain<nn::nex::Packet*,nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*>>*, unsigned short)
{
}

} // namespace nex
} // namespace nn
