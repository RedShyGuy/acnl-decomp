#include "nn/socket/detail/socket_DnsUserClient.h"

namespace nn {
namespace socket {
namespace detail {
// ctor candidate(s) 0x0048746C (unverified)
nn::socket::detail::DnsUserClient::DnsUserClient()
{
}

// 0x004862E8 slot 0x00 | virtual slot, introduced by nn::socket::detail::DnsUserClient
void nn::socket::detail::DnsUserClient::vf_0x00()
{
}

// 0x004862D8 slot 0x04 | virtual slot, introduced by nn::socket::detail::DnsUserClient
void nn::socket::detail::DnsUserClient::vf_0x04()
{
}

// 0x00485AA4 | fefates:bytes [tier B]
void nn::socket::detail::DnsUserClient::GetAddrInfo(const char*, const char*, const nn::socket::AddrInfo*, nn::socket::AddrInfo**)
{
}

// 0x00485EE4 | fefates:bytes [tier B]
void nn::socket::detail::DnsUserClient::FreeAddrInfo(nn::socket::AddrInfo*)
{
}

// 0x00485F3C | fefates:bytes [tier B]
void nn::socket::detail::DnsUserClient::GetHostByName(const char*)
{
}

// 0x00486054 | fefates:bytes [tier B]
void nn::socket::detail::DnsUserClient::MakeHostEntry()
{
}

} // namespace detail
} // namespace socket
} // namespace nn
