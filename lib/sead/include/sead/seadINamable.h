#pragma once

#include "decomp.h"
#include "sead/seadSafeStringBase.h"

namespace sead {
// RTTI N4sead8INamableE @ 0x008D21F8
//
// The name of an object (a SafeStringBase that points to the characters of the creator). The
// member name is ours.
class INamable
{
public:
    INamable(); // ctor address unknown (for the classes that are not decompiled yet)
    explicit INamable(const SafeStringBase<char>& name) : mINamableName(name) {}

    const SafeStringBase<char>& getName() const { return mINamableName; }

    SafeStringBase<char> mINamableName; // 0x0
};
ASSERT_SIZE(INamable, 0x8);
} // namespace sead
