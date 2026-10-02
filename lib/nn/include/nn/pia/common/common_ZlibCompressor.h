#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common14ZlibCompressorE @ 0x008CFE44
// vtable 0x00901538 (vptr 0x00901540), offset_to_top 0, 1 entries
class ZlibCompressor : public ::nn::pia::common::RootObject
{
public:
    virtual void vf_0x00(); // 0x00731AEC slot 0x00 | virtual slot, introduced by nn::pia::common::ZlibCompressor
    void Initialize(void*, unsigned int); // 0x00426F70 | fefates:bytes [tier B]
    void FinishDeflate(unsigned int*); // 0x00426FDC | fefates:bytes [tier B]
    void myFree(void*, void*); // 0x0042706C | fefates:bytes [tier B]
    void Cleanup(); // 0x00427080 | fefates:bytes [tier B]
    void Deflate(const unsigned char*, unsigned int); // 0x0042709C | fefates:bytes [tier B]
    void Startup(unsigned char*, unsigned int, int, int, int); // 0x0042711C | fefates:bytes [tier B]
    void myAlloc(void*, unsigned int, unsigned int); // 0x0042723C | fefates:bytes [tier B]
    ZlibCompressor(); // 0x00427294 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
