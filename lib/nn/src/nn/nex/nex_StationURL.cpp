#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_StationURL.h"

namespace nn {
namespace nex {
// 0x003565E8 slot 0x00 | mk7dlp:callseq-callee
nn::nex::StationURL::~StationURL()
{
}

// 0x00355224 | mk7dlp:bytes [tier A]
void nn::nex::StationURL::SetURLType(nn::nex::StationURL::URLType)
{
}

// 0x0035524C | fefates:bytes [tier B]
void nn::nex::StationURL::ParseParams(wchar_t*, bool)
{
}

// 0x00355534 | fefates:bytes-fuzzy [tier B]
void nn::nex::StationURL::SetParamValue(const wchar_t*, unsigned int)
{
}

// 0x00355754 | fefates:bytes [tier B]
void nn::nex::StationURL::SetPortNumber(unsigned short)
{
}

// 0x003557A4 | mk7dlp:callseq-callee [tier A]
void nn::nex::StationURL::SetInetAddress(const nn::nex::InetAddress*)
{
}

// 0x00355884 | fefates:bytes [tier B]
void nn::nex::StationURL::SetPlatformType()
{
}

// 0x003558FC | fefates:bytes [tier B]
void nn::nex::StationURL::SetRelayServerAddress(const nn::nex::String&)
{
}

// 0x003559A0 | fefates:bytes [tier B]
void nn::nex::StationURL::Copy(const nn::nex::StationURL&)
{
}

// 0x00355C54 | mk7dlp:callgraph [tier A]
void nn::nex::StationURL::Parse()
{
}

// 0x00355E38 | mk7dlp:callgraph [tier A]
void nn::nex::StationURL::Format()
{
}

// 0x00356344 | mk7dlp:callgraph [tier A]
void nn::nex::StationURL::SetURL(const wchar_t*)
{
}

// 0x00356414 | fefates:bytes [tier B]
nn::nex::StationURL::StationURL(const wchar_t*)
{
}

// 0x0035649C | mk7dlp:callseq-callee [tier A]
nn::nex::StationURL::StationURL(const nn::nex::StationURL&)
{
}

// 0x00356524 | mk7dlp:callseq-callee [tier A]
nn::nex::StationURL::StationURL()
{
}

// 0x0035684C | mk7dlp:bytes [tier A]
void nn::nex::StationURL::operator =(const nn::nex::String&)
{
}

// 0x00729A88 | mk7dlp:callseq-callee [tier A]
void nn::nex::StationURL::GetAddress() const
{
}

// 0x00729C20 | mk7dlp:bytes [tier A]
u8 nn::nex::StationURL::GetURLType() const
{
}

// 0x00729C3C | mk7dlp:callseq-callee [tier A]
u8 nn::nex::StationURL::GetStreamID() const
{
}

// 0x00729C58 | mk7dlp:bytes [tier A]
void nn::nex::StationURL::ValidateData() const
{
}

// 0x00729C88 | mk7dlp:callseq [tier A]
void nn::nex::StationURL::GetParamValue(const nn::nex::String&, unsigned) const
{
}

// 0x00729DF0 | mk7dlp:callseq-callee [tier A]
void nn::nex::StationURL::GetPortNumber() const
{
}

// 0x00729E30 | mk7dlp:bytes [tier A]
const nn::nex::InetAddress* nn::nex::StationURL::GetInetAddress() const
{
}

// 0x00729E4C | mk7dlp:callseq-callee [tier A]
u32 nn::nex::StationURL::GetPrincipalID() const
{
}

// 0x00729E5C | mk7dlp:callseq-callee [tier A]
u32 nn::nex::StationURL::GetConnectionID() const
{
}

// 0x0072A040 | fefates:bytes [tier B]
u8 nn::nex::StationURL::GetProbeRequestInitiation() const
{
}

// 0x0072A060 | mk7dlp:bytes [tier A]
void nn::nex::StationURL::GetURL() const
{
}

// 0x0072A08C | fefates:bytes [tier B]
void nn::nex::StationURL::operator!=(const nn::nex::StationURL&) const
{
}

} // namespace nex
} // namespace nn
