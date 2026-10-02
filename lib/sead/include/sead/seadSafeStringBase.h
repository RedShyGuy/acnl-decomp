#pragma once

#include "decomp.h"

namespace sead {
// Instantiations found in the binary:
//   sead::SafeStringBase<char>  typeinfo 0x008D1758  vtable 0x0090591C
//   sead::SafeStringBase<char>::iterator  typeinfo 0x008D1750  vtable 0x0090590C
//   sead::SafeStringBase<wchar_t>  typeinfo 0x008D1774  vtable 0x00905940
//   sead::SafeStringBase<wchar_t>::iterator  typeinfo 0x008D176C
//   sead::SafeStringBase<wchar_t>::token_iterator  typeinfo 0x008D1760  vtable 0x00905930
template <typename T0>
class SafeStringBase
{
public:
    // TODO: members unknown
};
} // namespace sead
