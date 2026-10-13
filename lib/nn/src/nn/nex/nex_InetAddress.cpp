#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_StringConversion.h"
#include "nn/nex/nex_StringStream.h"
#include "nn/socket/socket_Api.h"
#include "nn/socket/socket_Types.h"
#include <cwchar>

namespace nn {
namespace nex {
namespace {
// the sizes of the text forms of an address
const u32 ADDRESS_LENGTH = 20;
const u32 ADDRESS_WIDE_LENGTH = 128;
const u32 HOST_NAME_LENGTH = 256;
const u32 PORT_LENGTH = 16;
} // namespace

// 0x0096C922 (name is ours)
bool InetAddress::s_ResolvesHostNames = true;

// 0x003576F8 | mk7dlp:callgraph [tier A]
bool nn::nex::InetAddress::SetAddress(const wchar_t* pAddress)
{
    if (wcscmp(pAddress, L"255.255.255.255") == 0) {
        m_SockAddr.m_Address = 0xFFFFFFFF;
        return true;
    }
    if (wcscmp(pAddress, L"localhost") == 0) {
        return SetAddress(L"127.0.0.1");
    }
    char hostName[HOST_NAME_LENGTH];
    StringConversion::T2Char8(pAddress, hostName, HOST_NAME_LENGTH);
    u32 address;
    m_SockAddr.m_Address = nn::socket::InetPtoN(nn::socket::AF_INET, hostName, &address) > 0 ? address : 0;
    if (s_ResolvesHostNames && m_SockAddr.m_Address == 0) {
        nn::socket::HostEnt* pHost = nn::socket::GetHostByName(hostName);
        if (pHost != nullptr) {
            m_SockAddr.m_Address = **reinterpret_cast<u32**>(pHost->addrList);
            char text[ADDRESS_LENGTH];
            wchar_t wideText[ADDRESS_WIDE_LENGTH / 2];
            nn::socket::InetNtoP(nn::socket::AF_INET, &m_SockAddr.m_Address, text, ADDRESS_LENGTH);
            // (for a trace that is not in this build)
            StringConversion::Char8_2T(text, wideText, ADDRESS_WIDE_LENGTH / 2);
        }
    }
    return true;
}

// 0x0035783C | fefates:callgraph [tier C]
void nn::nex::InetAddress::SetAddress(unsigned int address)
{
    m_SockAddr.m_Address = __builtin_bswap32(address);
}

// 0x00357870 | fefates:callseq-callee [tier C]
void nn::nex::InetAddress::SetPortNumber(unsigned short port)
{
    m_SockAddr.m_Port = __builtin_bswap16(port);
}

// 0x0035787C | mk7dlp:bytes [tier A]
nn::nex::InetAddress::InetAddress(const nn::nex::InetAddress& other) : m_SockAddr(other.m_SockAddr)
{
}

// 0x0035789C | mk7dlp:bytes [tier A]
nn::nex::InetAddress::InetAddress(unsigned int address, unsigned short port)
{
    *reinterpret_cast<u32*>(&m_SockAddr) = 0;
    m_SockAddr.m_Family = nn::socket::AF_INET;
    m_SockAddr.m_Address = __builtin_bswap32(address);
    m_SockAddr.m_Port = __builtin_bswap16(port);
}

// 0x003578CC | mk7dlp:bytes [tier A]
nn::nex::InetAddress::InetAddress()
{
    *reinterpret_cast<u32*>(&m_SockAddr) = 0;
    m_SockAddr.m_Address = 0;
    m_SockAddr.m_Family = nn::socket::AF_INET;
}

// 0x00357908
// 0x003578F0 (deleting dtor)
nn::nex::InetAddress::~InetAddress()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0035790C | mk7dlp:bytes [tier A]
nn::nex::InetAddress& nn::nex::InetAddress::operator=(const nn::nex::InetAddress& other)
{
    m_SockAddr = other.m_SockAddr;
    return *this;
}

// 0x0072A11C | fefates:callgraph [tier C]
u32 nn::nex::InetAddress::GetAddress() const
{
    return __builtin_bswap32(m_SockAddr.m_Address);
}

// 0x0072A128 | fefates:bytes [tier B]
nn::nex::String nn::nex::InetAddress::GetAddressStr() const
{
    wchar_t wideText[ADDRESS_WIDE_LENGTH];
    char text[ADDRESS_LENGTH];
    nn::socket::InetNtoP(nn::socket::AF_INET, &m_SockAddr.m_Address, text, ADDRESS_LENGTH);
    StringConversion::Char8_2T(text, wideText, ADDRESS_WIDE_LENGTH);
    return String(wideText);
}

// 0x0072A180 | fefates:callgraph [tier C]
u16 nn::nex::InetAddress::GetPortNumber() const
{
    return __builtin_bswap16(m_SockAddr.m_Port);
}

// 0x0072A18C | fefates:bytes [tier B]
void nn::nex::InetAddress::ToStr(wchar_t* pString) const
{
    wchar_t wideText[ADDRESS_WIDE_LENGTH];
    char text[ADDRESS_LENGTH];
    nn::socket::InetNtoP(nn::socket::AF_INET, &m_SockAddr.m_Address, text, ADDRESS_LENGTH);
    StringConversion::Char8_2T(text, wideText, ADDRESS_WIDE_LENGTH);
    // (for a trace that is not in this build)
    wchar_t unusedPort[PORT_LENGTH];
    swprintf(unusedPort, PORT_LENGTH, L"%d", __builtin_bswap16(m_SockAddr.m_Port));
    StringStream stream;
    wchar_t port[PORT_LENGTH];
    stream << wideText << L":";
    swprintf(port, PORT_LENGTH, L"%d", __builtin_bswap16(m_SockAddr.m_Port));
    stream << String(port);
    wcscpy(pString, stream.CStr());
}

} // namespace nex
} // namespace nn
