#pragma once

#include "decomp.h"

namespace nn {
namespace socket {
namespace detail {
// RTTI N2nn6socket6detail13DnsUserClientE @ 0x008D0500
// vtable 0x00902328 (vptr 0x00902330), offset_to_top 0, 2 entries
class DnsUserClient
{
public:
    DnsUserClient(); // ctor candidate(s) 0x0048746C (unverified)
    virtual void vf_0x00(); // 0x004862E8 slot 0x00 | virtual slot, introduced by nn::socket::detail::DnsUserClient
    virtual void vf_0x04(); // 0x004862D8 slot 0x04 | virtual slot, introduced by nn::socket::detail::DnsUserClient
    void GetAddrInfo(const char*, const char*, const nn::socket::AddrInfo*, nn::socket::AddrInfo**); // 0x00485AA4 | fefates:bytes [tier B]
    void FreeAddrInfo(nn::socket::AddrInfo*); // 0x00485EE4 | fefates:bytes [tier B]
    void GetHostByName(const char*); // 0x00485F3C | fefates:bytes [tier B]
    void MakeHostEntry(); // 0x00486054 | fefates:bytes [tier B]
};
} // namespace detail
} // namespace socket
} // namespace nn
