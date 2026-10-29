#pragma once

#include "decomp.h"
#include "pead/peadFixedSafeStringBase.h"

namespace pead {
// Instantiations found in the binary:
//   pead::FixedSafeString<1024>  typeinfo 0x008D11B4  vtable 0x00904AC0
//   pead::FixedSafeString<128>  typeinfo 0x008D11C0  vtable 0x00904AD4
//   pead::FixedSafeString<32>  typeinfo 0x008D11CC  vtable 0x00904AE8
//
// A string of up to N - 1 characters with its own buffer.
template <int N>
class FixedSafeString : public FixedSafeStringBase<char, N>
{
public:
    FixedSafeString() {}
    explicit FixedSafeString(const SafeStringBase<char>& str) : FixedSafeStringBase<char, N>(str) {}
    virtual ~FixedSafeString() {}
};

// called, not inlined, in the original (TextStreamFormat)
template <>
DECOMP_NOINLINE FixedSafeString<32>::FixedSafeString();
} // namespace pead
