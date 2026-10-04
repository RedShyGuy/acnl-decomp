#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace pia {
namespace common {
class String;

// An IPv4 address and a port. Layout from the constructors; the member names are ours.
class InetAddress
{
public:
    // the serialized size (address and port, big endian)
    static const u32 SERIALIZED_SIZE = 6;

    InetAddress(); // 0x00426C40 | fefates:callgraph [tier C]
    InetAddress(unsigned int address, unsigned short port); // 0x00426C34 | fefates:callgraph [tier C]
    InetAddress(const nn::pia::common::InetAddress& rhs); // 0x00426C20 | fefates:callgraph [tier C]
    InetAddress& operator=(const nn::pia::common::InetAddress& rhs); // 0x00426C54 | fefates:callgraph [tier C]
    // empty (the "bx lr" between the constructors and operator=; armlink replaced the calls by nop)
    ~InetAddress() {} // 0x00426C50

    nn::Result Deserialize(const unsigned char* pBuffer); // 0x00426BD4 | fefates:bytes [tier B]
    nn::Result Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const; // 0x00731914 | fefates:bytes [tier B]
    // "a.b.c.d:port"
    void GetAddressString(nn::pia::common::String* pString) const; // 0x0073182C | fefates:bytes [tier B]
    // address in the high, port in the low half (for sorting; compared signed)
    s64 GetKey() const; // 0x00731888 | fefates:bytes [tier B]
    bool IsValid() const; // 0x007318A4 | fefates:bytes [tier B]
    bool IsValidAddress() const; // 0x0073181C | fefates:callgraph [tier C]
    // 10.0.0.0/8, 172.16.0.0/12 or 192.168.0.0/16
    bool IsPrivate() const; // 0x007318C4 | fefates:bytes [tier B]

    u32 GetAddress() const { return m_Address; }
    u16 GetPort() const { return m_Port; }

    u32 m_Address; // 0x0
    u16 m_Port;    // 0x4
};
ASSERT_SIZE(InetAddress, 0x8);
} // namespace common
} // namespace pia
} // namespace nn
