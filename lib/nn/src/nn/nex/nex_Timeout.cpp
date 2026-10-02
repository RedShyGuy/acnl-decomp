#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Timeout.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::Timeout::Timeout()
{
}

// 0x003D3814 slot 0x00 | virtual slot, introduced by nn::nex::Timeout
void nn::nex::Timeout::vf_0x00()
{
}

// 0x003D37F0 slot 0x04 | virtual slot, introduced by nn::nex::Timeout
void nn::nex::Timeout::vf_0x04()
{
}

// 0x003D3794 | mk7dlp:bytes [tier A]
void nn::nex::Timeout::SetRelativeExpirationTime(int)
{
}

// 0x0072E478 | fefates:bytes [tier B]
void nn::nex::Timeout::IsExpired() const
{
}

} // namespace nex
} // namespace nn
