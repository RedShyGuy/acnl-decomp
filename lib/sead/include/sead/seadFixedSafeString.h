#pragma once

#include "decomp.h"

namespace sead {
// Instantiations found in the binary:
//   sead::FixedSafeString<1024>  typeinfo 0x008D17A8  vtable 0x00905970
//   sead::FixedSafeString<128>  typeinfo 0x008D17B4  vtable 0x00905984
//   sead::FixedSafeString<16>  typeinfo 0x008D17C0  vtable 0x00905998
//   sead::FixedSafeString<17>  typeinfo 0x008D17CC  vtable 0x009059AC
//   sead::FixedSafeString<20>  typeinfo 0x008D17D8  vtable 0x009059C0
//   sead::FixedSafeString<256>  typeinfo 0x008D17E4  vtable 0x009059D4
//   sead::FixedSafeString<2>  typeinfo 0x008D17F0  vtable 0x009059E8
//   sead::FixedSafeString<30>  typeinfo 0x008D17FC  vtable 0x009059FC
//   sead::FixedSafeString<32>  typeinfo 0x008D1808  vtable 0x00905A10
//   sead::FixedSafeString<33>  typeinfo 0x008D1814  vtable 0x00905A24
//   sead::FixedSafeString<48>  typeinfo 0x008D1820  vtable 0x00905A38
//   sead::FixedSafeString<4>  typeinfo 0x008D182C  vtable 0x00905A4C
//   sead::FixedSafeString<512>  typeinfo 0x008D1838  vtable 0x00905A60
//   sead::FixedSafeString<64>  typeinfo 0x008D1844  vtable 0x00905A74
//   sead::FixedSafeString<6>  typeinfo 0x008D1850  vtable 0x00905A88
//   sead::FixedSafeString<7>  typeinfo 0x008D185C  vtable 0x00905A9C
//   sead::FixedSafeString<80>  typeinfo 0x008D1868  vtable 0x00905AB0
//   sead::FixedSafeString<8>  typeinfo 0x008D1874  vtable 0x00905AC4
//   sead::FixedSafeString<96>  typeinfo 0x008D1880  vtable 0x00905AD8
template <auto T0>
class FixedSafeString
{
public:
    // TODO: members unknown
};
} // namespace sead
