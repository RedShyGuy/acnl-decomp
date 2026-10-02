#include "nn/nex/nex_DateTime.h"

namespace nn {
namespace nex {
// TODO: default ctor added so derived stubs compile - may not exist
nn::nex::DateTime::DateTime()
{
}

// 0x003D494C | fefates:bytes [tier B]
void nn::nex::DateTime::GetSystemTime(nn::nex::DateTime&)
{
}

// 0x003D4B4C | mk7dlp:bytes-fuzzy [tier A]
nn::nex::DateTime::DateTime(unsigned short, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char)
{
}

// 0x0072E644 | fefates:bytes [tier B]
void nn::nex::DateTime::ToEpochTime() const
{
}

// 0x0072E9AC | fefates:bytes [tier B]
void nn::nex::DateTime::operator-(const nn::nex::DateTime&) const
{
}

} // namespace nex
} // namespace nn
