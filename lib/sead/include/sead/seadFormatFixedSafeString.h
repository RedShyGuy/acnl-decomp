#pragma once

#include "decomp.h"
#include "sead/seadFixedSafeString.h"

namespace sead {
// Instantiations found in the binary:
//   sead::FormatFixedSafeString<128>  typeinfo 0x008D1EA8  vtable 0x009065D4
//   sead::FormatFixedSafeString<32>  typeinfo 0x008D1EB4  vtable 0x009065E8
//   sead::FormatFixedSafeString<64>  typeinfo 0x008D1EC0  vtable 0x009065FC
//   sead::FormatFixedSafeString<6>  typeinfo 0x008D1ECC  vtable 0x00906610
//
// A FixedSafeString built from a format (the constructors of the four instances are called, see
// seadSafeString.cpp).
template <int N>
class FormatFixedSafeString : public FixedSafeString<N>
{
public:
    FormatFixedSafeString(const char* fmt, ...) : FixedSafeString<N>()
    {
        std::va_list args;
        va_start(args, fmt);
        this->formatV(fmt, args);
        va_end(args);
    }
    virtual ~FormatFixedSafeString() {}
};
template <>
DECOMP_NOINLINE FormatFixedSafeString<128>::FormatFixedSafeString(const char* fmt, ...);
template <>
DECOMP_NOINLINE FormatFixedSafeString<32>::FormatFixedSafeString(const char* fmt, ...);
template <>
DECOMP_NOINLINE FormatFixedSafeString<64>::FormatFixedSafeString(const char* fmt, ...);
template <>
DECOMP_NOINLINE FormatFixedSafeString<6>::FormatFixedSafeString(const char* fmt, ...);
} // namespace sead
