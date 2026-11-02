#pragma once

#include "decomp.h"
#include "sead/seadVector3.h"

namespace ssys {
namespace ma {
// a sead::Vector3<int> with a vtable (vptr at 0, x/y/z behind it)
// RTTI N4ssys2ma5Vec3iE @ 0x008D24D0
// vtable 0x00907368 (vptr 0x00907370), offset_to_top 0, 2 entries
class Vec3i : public ::sead::Vector3<int>
{
public:
    Vec3i() {} // (inline: the users only store the vptr)
    virtual ~Vec3i(); // 0x0056AEE0 slot 0x00
};
ASSERT_SIZE(Vec3i, 0x10);
} // namespace ma
} // namespace ssys
