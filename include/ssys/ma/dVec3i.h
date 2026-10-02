#pragma once

#include "decomp.h"
#include "sead/seadVector3.h"

namespace ssys {
namespace ma {
// RTTI N4ssys2ma5Vec3iE @ 0x008D24D0
// vtable 0x00907368 (vptr 0x00907370), offset_to_top 0, 2 entries
class Vec3i : public ::sead::Vector3<int>
{
public:
    Vec3i(); // ctor candidate(s) 0x006A9DE4, 0x00708B50 (unverified)
    virtual void vf_0x00(); // 0x0056AEE0 slot 0x00 | virtual slot, introduced by ssys::ma::Vec3i
    virtual void vf_0x04(); // 0x0056AEDC slot 0x04 | virtual slot, introduced by ssys::ma::Vec3i
};
} // namespace ma
} // namespace ssys
