#pragma once

#include "decomp.h"

namespace nn {
namespace nwm {
namespace CTR {
// RTTI N2nn3nwm3CTR9BssReaderE @ 0x008CF7EC
class BssReader
{
public:
    BssReader(); // ctor address unknown
    void GetVendorIe(const unsigned char*, unsigned char) const; // 0x0072EE00 | fefates:bytes [tier B]
};
} // namespace CTR
} // namespace nwm
} // namespace nn
