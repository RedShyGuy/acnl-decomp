#include "nn/pia/inet/inet_SocketStreamBase.h"
#include "nn/pia/common/common_IPacketInput.h"
#include "nn/pia/inet/inet_SocketInputStream.h"

namespace nn {
namespace pia {
namespace inet {
// 0x003E7FFC slot 0x00 | slot vf_0x00 of nn::pia::inet::SocketStreamBase
nn::pia::inet::SocketInputStream::~SocketInputStream()
{
}

// 0x003E8900 slot 0x04 | virtual slot, introduced by nn::pia::inet::SocketStreamBase
void nn::pia::inet::SocketInputStream::vf_0x04()
{
}

// 0x003E88C4 slot 0x08 | virtual slot, introduced by nn::pia::inet::SocketInputStream
void nn::pia::inet::SocketInputStream::vf_0x08()
{
}

// 0x003E88A8 slot 0x0C | virtual slot, introduced by nn::pia::inet::SocketInputStream
void nn::pia::inet::SocketInputStream::vf_0x0C()
{
}

// 0x003E88E0 | fefates:bytes [tier B]
nn::pia::inet::SocketInputStream::SocketInputStream()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
