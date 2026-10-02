#pragma once

#include "decomp.h"

namespace sead {
// Instantiations found in the binary:
//   sead::FormatFixedSafeString<128>  typeinfo 0x008D1EA8  vtable 0x009065D4
//   sead::FormatFixedSafeString<32>  typeinfo 0x008D1EB4  vtable 0x009065E8
//   sead::FormatFixedSafeString<64>  typeinfo 0x008D1EC0  vtable 0x009065FC
//   sead::FormatFixedSafeString<6>  typeinfo 0x008D1ECC  vtable 0x00906610
template <auto T0>
class FormatFixedSafeString
{
public:
    // TODO: members unknown
};
} // namespace sead
