#include "nn/pia/inet/inet_Socket.h"

namespace nn {
namespace pia {
namespace inet {
// 0x0041243C | fefates:bytes [tier B]
void nn::pia::inet::Socket::SendToMulti(const void*, int, const nn::pia::inet::SockAddrIn*, int, int*)
{
}

// 0x00412524 | fefates:bytes [tier B]
void nn::pia::inet::Socket::Bind(const nn::pia::common::InetAddress&)
{
}

// 0x00412650 | fefates:bytes [tier B]
void nn::pia::inet::Socket::Bind(unsigned short)
{
}

// 0x004126A4 | fefates:bytes [tier B]
void nn::pia::inet::Socket::Open(int, int)
{
}

// 0x004127A0 | fefates:bytes [tier B]
void nn::pia::inet::Socket::Close()
{
}

// 0x00412898 | fefates:bytes [tier B]
void nn::pia::inet::Socket::SendTo(const void*, int, const nn::pia::common::InetAddress&, int*)
{
}

// 0x0041295C | fefates:bytes [tier B]
void nn::pia::inet::Socket::SetTtl(unsigned char)
{
}

// 0x00412BCC | fefates:bytes [tier B]
nn::pia::inet::Socket::Socket()
{
}

// 0x00412BF4 | fefates:bytes [tier B]
nn::pia::inet::Socket::~Socket()
{
}

} // namespace inet
} // namespace pia
} // namespace nn
