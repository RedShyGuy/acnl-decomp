#include "nn/socket/detail/socket_User.h"

namespace nn {
namespace socket {
namespace detail {
// 0x004883DC | fefates:bytes [tier A]
void nn::socket::detail::User::SetSockOpt(int*, int, int, int, const unsigned char*, int)
{
}

// 0x00488444 | fefates:bytes [tier A]
void nn::socket::detail::User::GetAddrInfo(int*, const char*, unsigned int, const char*, unsigned int, const unsigned char*, int, int*, unsigned char*, unsigned int)
{
}

// 0x0048850C | fefates:bytes [tier A]
void nn::socket::detail::User::GetSockName(int*, int, unsigned char*, unsigned int)
{
}

// 0x00488578 | fefates:bytes [tier A]
void nn::socket::detail::User::SendToSmall(int*, int, const unsigned char*, int, int, const unsigned char*, unsigned int)
{
}

// 0x004885FC | fefates:callgraph [tier A]
void nn::socket::detail::User::AttachProcess(nn::Handle, unsigned int)
{
}

// 0x00488668 | fefates:bytes [tier A]
void nn::socket::detail::User::GetHostByName(int*, const char*, unsigned int, unsigned char*, unsigned int)
{
}

// 0x004886E4 | fefates:bytes [tier B]
void nn::socket::detail::User::GetNetworkOpt(int*, int, int, unsigned char*, int*)
{
}

// 0x00488760 | fefates:bytes [tier B]
void nn::socket::detail::User::RecvFromSmall(int*, int, unsigned char*, int*, int, unsigned char*, unsigned int)
{
}

// 0x00488804 | fefates:bytes [tier A]
void nn::socket::detail::User::SendToSmallMulti(int*, int, const unsigned char*, int, int, const unsigned char*, unsigned int, unsigned int)
{
}

// 0x0048887C | fefates:bytes [tier A]
void nn::socket::detail::User::Bind(int*, int, const unsigned char*, unsigned int)
{
}

// 0x004888DC | fefates:bytes [tier B]
void nn::socket::detail::User::Poll(int*, const nn::socket::PollFd*, nn::socket::PollFd*, unsigned int, int)
{
}

// 0x00488968 | fefates:bytes [tier B]
void nn::socket::detail::User::Close(int*, int)
{
}

// 0x004889AC | fefates:bytes [tier A]
void nn::socket::detail::User::Fcntl(int*, int, int, int)
{
}

// 0x004889F8 | fefates:bytes [tier A]
void nn::socket::detail::User::SendTo(int*, int, const unsigned char*, int, int, const unsigned char*, unsigned int)
{
}

// 0x00488A7C | fefates:bytes [tier A]
void nn::socket::detail::User::Socket(int*, int, int, int)
{
}

// 0x00488B28 | fefates:bytes [tier A]
void nn::socket::detail::User::RecvFrom(int*, int, unsigned char*, int, int, unsigned char*, unsigned int)
{
}

// 0x00488BAC | fefates:bytes [tier B]
void nn::socket::detail::User::Shutdown(int*, int, int)
{
}

// 0x00488BF0 | fefates:bytes [tier B]
void nn::socket::detail::User::GetHostId(unsigned int*)
{
}

} // namespace detail
} // namespace socket
} // namespace nn
