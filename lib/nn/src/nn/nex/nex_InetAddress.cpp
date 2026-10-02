#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_InetAddress.h"

namespace nn {
namespace nex {
// 0x00357908 slot 0x00 | virtual slot, introduced by nn::nex::InetAddress
void nn::nex::InetAddress::vf_0x00()
{
}

// 0x003578F0 slot 0x04 | virtual slot, introduced by nn::nex::InetAddress
void nn::nex::InetAddress::vf_0x04()
{
}

// 0x003576F8 | mk7dlp:callgraph [tier A]
void nn::nex::InetAddress::SetAddress(const wchar_t*)
{
}

// 0x0035787C | mk7dlp:bytes [tier A]
nn::nex::InetAddress::InetAddress(const nn::nex::InetAddress&)
{
}

// 0x0035789C | mk7dlp:bytes [tier A]
nn::nex::InetAddress::InetAddress(unsigned, unsigned short)
{
}

// 0x003578CC | mk7dlp:bytes [tier A]
nn::nex::InetAddress::InetAddress()
{
}

// 0x0035790C | mk7dlp:bytes [tier A]
void nn::nex::InetAddress::operator =(const nn::nex::InetAddress&)
{
}

// 0x0072A128 | fefates:bytes [tier B]
void nn::nex::InetAddress::GetAddressStr() const
{
}

// 0x0072A18C | fefates:bytes [tier B]
void nn::nex::InetAddress::ToStr(wchar_t*) const
{
}

} // namespace nex
} // namespace nn
