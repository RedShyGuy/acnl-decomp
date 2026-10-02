#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class DateTime
{
public:
    DateTime(); // TODO: default ctor added so derived stubs compile - may not exist
    void GetSystemTime(nn::nex::DateTime&); // 0x003D494C | fefates:bytes [tier B]
    DateTime(unsigned short, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char); // 0x003D4B4C | mk7dlp:bytes-fuzzy [tier A]
    void ToEpochTime() const; // 0x0072E644 | fefates:bytes [tier B]
    void operator-(const nn::nex::DateTime&) const; // 0x0072E9AC | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
