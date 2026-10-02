#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace inet {
class SocketAddress
{
public:
    void GetAddressInfo(const char*, const char*, const nn::pia::inet::AddrInfo*); // 0x003E6BC4 | fefates:bytes [tier B]
    void GetInetAddress(nn::pia::common::InetAddress*); // 0x003E6C10 | fefates:bytes [tier B]
    void SetInetAddress(const nn::pia::common::InetAddress&); // 0x003E6C50 | fefates:bytes [tier B]
    void Init(); // 0x003E6C7C | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
