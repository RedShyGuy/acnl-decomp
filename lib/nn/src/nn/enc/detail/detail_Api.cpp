#include "nn/enc/detail/detail_Api.h"

namespace nn {
namespace enc {
namespace detail {
// 0x00351B3C | fefates:bytes [tier B]
void CheckBreakType(unsigned int, unsigned int)
{
}

// 0x00351B64 | fefates:bytes [tier B]
void WriteBreakType(unsigned char*, unsigned int, nn::enc::BreakType, bool)
{
}

// 0x00351BF8 | fefates:bytes [tier B]
void CheckParameters(bool, int*, int*, bool*, bool, int*, int*, bool*)
{
}

// 0x00351C84 | fefates:bytes [tier B]
void ConvertStringUtf16NativeToUtf8(unsigned char*, int*, const unsigned short*, int*, nn::enc::BreakType)
{
}

// 0x0035201C | fefates:bytes [tier B]
void ConvertStringUtf8ToUtf16Native(unsigned short*, int*, const unsigned char*, int*, nn::enc::BreakType)
{
}

} // namespace detail
} // namespace enc
} // namespace nn
