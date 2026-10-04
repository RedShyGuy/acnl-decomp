#pragma once

#include "decomp.h"

namespace pead {
// Instantiations found in the binary:
//   pead::SafeStringBase<char>  typeinfo 0x008D1198  vtable 0x00904A88
//   pead::SafeStringBase<char>::iterator  typeinfo 0x008D1190  vtable 0x00904A78
//   pead::SafeStringBase<wchar_t>  typeinfo 0x008D11A0  vtable 0x00904A9C
//
// A string pointer with a vtable. pia builds them on the stack from a literal (vptr, pointer),
// e.g. the heap name in common::Initialize; the destructor is empty (ARMCC removed the call).
// The member names are ours.
template <typename T0>
class SafeStringBase
{
public:
    SafeStringBase(const T0* str) : mStringTop(str) {}
    virtual ~SafeStringBase() {} // <char>: 0x00810054 slot 0x00
    // <char>: 0x00810050 slot 0x04 (deleting dtor)
    virtual void vf_0x08() const; // <char>: 0x00828238 slot 0x08

    const T0* mStringTop; // 0x04
};
} // namespace pead
