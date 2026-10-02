#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace inet {
class Socket
{
public:
    void SendToMulti(const void*, int, const nn::pia::inet::SockAddrIn*, int, int*); // 0x0041243C | fefates:bytes [tier B]
    void Bind(const nn::pia::common::InetAddress&); // 0x00412524 | fefates:bytes [tier B]
    void Bind(unsigned short); // 0x00412650 | fefates:bytes [tier B]
    void Open(int, int); // 0x004126A4 | fefates:bytes [tier B]
    void Close(); // 0x004127A0 | fefates:bytes [tier B]
    void SendTo(const void*, int, const nn::pia::common::InetAddress&, int*); // 0x00412898 | fefates:bytes [tier B]
    void SetTtl(unsigned char); // 0x0041295C | fefates:bytes [tier B]
    Socket(); // 0x00412BCC | fefates:bytes [tier B]
    ~Socket(); // 0x00412BF4 | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
