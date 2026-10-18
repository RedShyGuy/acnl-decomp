#include "nn/nex/nex_InterfaceInfo.h"
#include "nn/nex/nex_StringConversion.h"
#include "nn/socket/socket_Api.h"
#include "nn/socket/socket_Types.h"

#include <cwchar>

namespace nn {
namespace nex {
namespace {
// the sizes of the texts
const u32 TEXT_LENGTH = 128;
const u32 ADDRESS_LENGTH = 20;
const u32 FLAGS_LENGTH = 512;
} // namespace

// 0x00361DDC | mk7dlp:bytes-fuzzy [tier A]
void nn::nex::InterfaceInfo::Trace(unsigned long long traceFlags)
{
    wchar_t text[TEXT_LENGTH];
    if (m_Name.m_pString != nullptr) {
        wcsncpy(text, m_Name.m_pString, TEXT_LENGTH);
        text[TEXT_LENGTH - 1] = L'\0';
    }
    {
        char addressText[ADDRESS_LENGTH];
        u32 address = m_Address;
        nn::socket::InetNtoP(nn::socket::AF_INET, &address, addressText, ADDRESS_LENGTH);
        StringConversion::Char8_2T(addressText, text, TEXT_LENGTH);
    }
    {
        char addressText[ADDRESS_LENGTH];
        u32 address = m_BroadcastAddress;
        nn::socket::InetNtoP(nn::socket::AF_INET, &address, addressText, ADDRESS_LENGTH);
        StringConversion::Char8_2T(addressText, text, TEXT_LENGTH);
    }
    {
        char addressText[ADDRESS_LENGTH];
        u32 address = m_Netmask;
        nn::socket::InetNtoP(nn::socket::AF_INET, &address, addressText, ADDRESS_LENGTH);
        StringConversion::Char8_2T(addressText, text, TEXT_LENGTH);
    }
    wchar_t flagsText[FLAGS_LENGTH] = {};
    if (m_Flags & FLAG_POINT_TO_POINT) {
        wcscat(flagsText, L"POINT2POINT");
    }
    if (m_Flags & FLAG_LOOPBACK) {
        wcscat(flagsText, L"LOOPBACK ");
    }
    if (m_Flags & FLAG_BROADCAST) {
        wcscat(flagsText, L"BROADCAST ");
    }
    wcsncpy(text, flagsText, TEXT_LENGTH);
}

// 0x00361F24 | fefates:bytes [tier B]
nn::nex::InterfaceInfo::InterfaceInfo() : m_Name(L"")
{
    m_Address = 0;
    m_BroadcastAddress = 0;
    m_Netmask = 0;
    m_Flags = 0;
}

// 0x00361F64
nn::nex::InterfaceInfo::~InterfaceInfo()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
