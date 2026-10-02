#pragma once

#include "decomp.h"

namespace sead {
// Instantiations found in the binary:
//   sead::FixedSafeStringBase<char, 1024>  typeinfo 0x008D1C6C  vtable 0x009061CC
//   sead::FixedSafeStringBase<char, 128>  typeinfo 0x008D1C78  vtable 0x009061E0
//   sead::FixedSafeStringBase<char, 16>  typeinfo 0x008D1C84
//   sead::FixedSafeStringBase<char, 17>  typeinfo 0x008D1C90  vtable 0x009061F4
//   sead::FixedSafeStringBase<char, 20>  typeinfo 0x008D1C9C  vtable 0x00906208
//   sead::FixedSafeStringBase<char, 256>  typeinfo 0x008D1CA8  vtable 0x0090621C
//   sead::FixedSafeStringBase<char, 2>  typeinfo 0x008D1CB4  vtable 0x00906230
//   sead::FixedSafeStringBase<char, 30>  typeinfo 0x008D1CC0
//   sead::FixedSafeStringBase<char, 32>  typeinfo 0x008D1CCC  vtable 0x00906244
//   sead::FixedSafeStringBase<char, 33>  typeinfo 0x008D1CD8
//   sead::FixedSafeStringBase<char, 48>  typeinfo 0x008D1CE4  vtable 0x00906258
//   sead::FixedSafeStringBase<char, 4>  typeinfo 0x008D1CF0  vtable 0x0090626C
//   sead::FixedSafeStringBase<char, 512>  typeinfo 0x008D1CFC  vtable 0x00906280
//   sead::FixedSafeStringBase<char, 64>  typeinfo 0x008D1D08  vtable 0x00906294
//   sead::FixedSafeStringBase<char, 6>  typeinfo 0x008D1D14  vtable 0x009062A8
//   sead::FixedSafeStringBase<char, 7>  typeinfo 0x008D1D20  vtable 0x009062BC
//   sead::FixedSafeStringBase<char, 80>  typeinfo 0x008D1D2C
//   sead::FixedSafeStringBase<char, 8>  typeinfo 0x008D1D38  vtable 0x009062D0
//   sead::FixedSafeStringBase<char, 96>  typeinfo 0x008D1D44  vtable 0x009062E4
//   sead::FixedSafeStringBase<wchar_t, 13>  typeinfo 0x008D1D50
//   sead::FixedSafeStringBase<wchar_t, 18>  typeinfo 0x008D1D5C  vtable 0x009062F8
//   sead::FixedSafeStringBase<wchar_t, 197>  typeinfo 0x008D1D68  vtable 0x0090630C
//   sead::FixedSafeStringBase<wchar_t, 201>  typeinfo 0x008D1D74  vtable 0x00906320
//   sead::FixedSafeStringBase<wchar_t, 21>  typeinfo 0x008D1D80  vtable 0x00906334
//   sead::FixedSafeStringBase<wchar_t, 256>  typeinfo 0x008D1D8C
//   sead::FixedSafeStringBase<wchar_t, 32>  typeinfo 0x008D1D98
//   sead::FixedSafeStringBase<wchar_t, 33>  typeinfo 0x008D1DA4  vtable 0x00906348
//   sead::FixedSafeStringBase<wchar_t, 36>  typeinfo 0x008D1DB0  vtable 0x0090635C
//   sead::FixedSafeStringBase<wchar_t, 44>  typeinfo 0x008D1DBC  vtable 0x00906370
//   sead::FixedSafeStringBase<wchar_t, 50>  typeinfo 0x008D1DC8
//   sead::FixedSafeStringBase<wchar_t, 53>  typeinfo 0x008D1DD4
//   sead::FixedSafeStringBase<wchar_t, 5>  typeinfo 0x008D1DE0  vtable 0x00906384
//   sead::FixedSafeStringBase<wchar_t, 64>  typeinfo 0x008D1DEC  vtable 0x00906398
//   sead::FixedSafeStringBase<wchar_t, 65>  typeinfo 0x008D1DF8  vtable 0x009063AC
//   sead::FixedSafeStringBase<wchar_t, 6>  typeinfo 0x008D1E04  vtable 0x009063C0
//   sead::FixedSafeStringBase<wchar_t, 7>  typeinfo 0x008D1E10
//   sead::FixedSafeStringBase<wchar_t, 9>  typeinfo 0x008D1E1C
template <typename T0, auto T1>
class FixedSafeStringBase
{
public:
    // TODO: members unknown
};
} // namespace sead
