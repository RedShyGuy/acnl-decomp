#include "nn/pia/inet/inet_SocketStreamBase.h"
#include "nn/pia/common/common_IPacketOutput.h"
#include "nn/pia/inet/inet_SocketOutputStream.h"

namespace nn {
namespace pia {
namespace inet {
// 0x003F8A6C slot 0x00 | slot vf_0x00 of nn::pia::inet::SocketStreamBase
nn::pia::inet::SocketOutputStream::~SocketOutputStream()
{
}

// 0x003F8A5C slot 0x04 | virtual slot, introduced by nn::pia::inet::SocketStreamBase
void nn::pia::inet::SocketOutputStream::vf_0x04()
{
}

// 0x003F85F4 slot 0x08 | fefates:bytes
void nn::pia::inet::SocketOutputStream::OnStationConnectionEvent()
{
}

// 0x003F8868 slot 0x0C | virtual slot, introduced by nn::pia::inet::SocketOutputStream
void nn::pia::inet::SocketOutputStream::vf_0x0C()
{
}

// 0x003F86BC slot 0x10 | fefates:callseq
void nn::pia::inet::SocketOutputStream::vf_0x10()
{
}

// 0x0072F160 slot 0x14 | virtual slot, introduced by nn::pia::inet::SocketOutputStream
void nn::pia::inet::SocketOutputStream::vf_0x14()
{
}

// 0x0072F158 slot 0x18 | virtual slot, introduced by nn::pia::inet::SocketOutputStream
void nn::pia::inet::SocketOutputStream::vf_0x18()
{
}

// 0x003F8420 slot 0x1C | virtual slot, introduced by nn::pia::inet::SocketOutputStream
void nn::pia::inet::SocketOutputStream::vf_0x1C()
{
}

// 0x003F8578 | fefates:bytes [tier B]
void nn::pia::inet::SocketOutputStream::SendToMulti(const void*, unsigned int, const nn::pia::inet::SockAddrIn*, int)
{
}

// 0x003F894C | fefates:bytes [tier B]
void nn::pia::inet::SocketOutputStream::SendTo(const void*, unsigned int, const nn::pia::common::InetAddress&, unsigned char)
{
}

// 0x003F8A3C | fefates:bytes [tier B]
nn::pia::inet::SocketOutputStream::SocketOutputStream()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
