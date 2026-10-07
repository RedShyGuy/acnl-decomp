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
    // the fields of the packed value (inline; armlink placed them at the end of the code; names
    // are ours)
    u8 GetDay() const; // 0x0072E904
    u8 GetHour() const; // 0x0072E920
    u16 GetYear() const; // 0x0072E93C
    u8 GetMonth() const; // 0x0072E958
    u8 GetMinute() const; // 0x0072E974
    u8 GetSecond() const; // 0x0072E990

    // (the packed date and time; the name is ours, the size is from nex::Variant)
    u64 m_Value; // 0x0
};
} // namespace nex
} // namespace nn
