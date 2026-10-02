#pragma once

#include "decomp.h"

namespace pead {
// Instantiations found in the binary:
//   pead::FixedSafeString<1024>  typeinfo 0x008D11B4  vtable 0x00904AC0
//   pead::FixedSafeString<128>  typeinfo 0x008D11C0  vtable 0x00904AD4
//   pead::FixedSafeString<32>  typeinfo 0x008D11CC  vtable 0x00904AE8
template <auto T0>
class FixedSafeString
{
public:
    // TODO: members unknown
};
} // namespace pead
