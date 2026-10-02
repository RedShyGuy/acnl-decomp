#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_ZlibCompressor.h"

namespace nn {
namespace pia {
namespace common {
// 0x00731AEC slot 0x00 | virtual slot, introduced by nn::pia::common::ZlibCompressor
void nn::pia::common::ZlibCompressor::vf_0x00()
{
}

// 0x00426F70 | fefates:bytes [tier B]
void nn::pia::common::ZlibCompressor::Initialize(void*, unsigned int)
{
}

// 0x00426FDC | fefates:bytes [tier B]
void nn::pia::common::ZlibCompressor::FinishDeflate(unsigned int*)
{
}

// 0x0042706C | fefates:bytes [tier B]
void nn::pia::common::ZlibCompressor::myFree(void*, void*)
{
}

// 0x00427080 | fefates:bytes [tier B]
void nn::pia::common::ZlibCompressor::Cleanup()
{
}

// 0x0042709C | fefates:bytes [tier B]
void nn::pia::common::ZlibCompressor::Deflate(const unsigned char*, unsigned int)
{
}

// 0x0042711C | fefates:bytes [tier B]
void nn::pia::common::ZlibCompressor::Startup(unsigned char*, unsigned int, int, int, int)
{
}

// 0x0042723C | fefates:bytes [tier B]
void nn::pia::common::ZlibCompressor::myAlloc(void*, unsigned int, unsigned int)
{
}

// 0x00427294 | fefates:bytes [tier B]
nn::pia::common::ZlibCompressor::ZlibCompressor()
{
}

} // namespace common
} // namespace pia
} // namespace nn
