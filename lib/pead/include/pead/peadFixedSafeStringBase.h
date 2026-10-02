#pragma once

#include "decomp.h"

namespace pead {
// Instantiations found in the binary:
//   pead::FixedSafeStringBase<char, 1024>  typeinfo 0x008D121C  vtable 0x00904C18
//   pead::FixedSafeStringBase<char, 128>  typeinfo 0x008D1228  vtable 0x00904C2C
//   pead::FixedSafeStringBase<char, 32>  typeinfo 0x008D1234  vtable 0x00904C40
template <typename T0, auto T1>
class FixedSafeStringBase
{
public:
    // TODO: members unknown
};
} // namespace pead
