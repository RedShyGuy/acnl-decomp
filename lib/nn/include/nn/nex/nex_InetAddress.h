#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// An IPv4 address and port, kept as a socket address (network byte order). The names of the
// members are ours.
// RTTI N2nn3nex11InetAddressE @ 0x008CE018
// vtable 0x008FC2AC (vptr 0x008FC2B4), offset_to_top 0, 2 entries
class InetAddress : public ::nn::nex::RootObject
{
public:
    // the socket address (like sockaddr_in, with more room behind it)
    struct SockAddr
    {
        u8 m_Length;           // 0x00
        u8 m_Family;           // 0x01
        u16 m_Port;            // 0x02 (network byte order)
        u32 m_Address;         // 0x04 (network byte order)
        u8 m_Unknown0x08[20];  // 0x08
    };

    InetAddress(); // 0x003578CC | mk7dlp:bytes [tier A]
    InetAddress(unsigned int address, unsigned short port); // 0x0035789C | mk7dlp:bytes [tier A]
    InetAddress(const nn::nex::InetAddress& other); // 0x0035787C | mk7dlp:bytes [tier A]
    virtual ~InetAddress(); // 0x00357908 slot 0x00
    // 0x003578F0 slot 0x04 (deleting dtor)
    nn::nex::InetAddress& operator=(const nn::nex::InetAddress& other); // 0x0035790C | mk7dlp:bytes [tier A]

    // a dotted address, "255.255.255.255", "localhost" or (if s_ResolvesHostNames) a host name
    bool SetAddress(const wchar_t* pAddress); // 0x003576F8 | mk7dlp:callgraph [tier A]
    void SetAddress(unsigned int address); // 0x0035783C | fefates:callgraph [tier C]
    void SetPortNumber(unsigned short port); // 0x00357870 | fefates:callseq-callee [tier C]
    // (inline in the original, with copies out of line)
    u32 GetAddress() const; // 0x0072A11C | fefates:callgraph [tier C]
    nn::nex::String GetAddressStr() const; // 0x0072A128 | fefates:bytes [tier B]
    u16 GetPortNumber() const; // 0x0072A180 | fefates:callgraph [tier C]
    // "address:port"
    void ToStr(wchar_t* pString) const; // 0x0072A18C | fefates:bytes [tier B]

    SockAddr m_SockAddr; // 0x04

    // SetAddress looks up host names (name is ours)
    static bool s_ResolvesHostNames;
};
ASSERT_SIZE(InetAddress, 0x20);
} // namespace nex
} // namespace nn
