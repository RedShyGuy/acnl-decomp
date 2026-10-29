#pragma once

#include "decomp.h"
#include "pead/peadSafeStringBase.h"

namespace pead {
// RTTI N4pead8INamableE @ 0x008D1304
//
// The name of an object (pia's copy of sead::INamable: a SafeStringBase that points to the characters of the
// creator). The member name is ours.
class INamable
{
public:
    explicit INamable(const SafeStringBase<char>& name) : mINamableName(name) {}

    const SafeStringBase<char>& getName() const { return mINamableName; }

    SafeStringBase<char> mINamableName; // 0x0
};
ASSERT_SIZE(INamable, 0x8);
} // namespace pead
