#pragma once

#include "decomp.h"

namespace sead {
// Instantiations found in the binary:
//   sead::Vector3<int>  typeinfo 0x008D2164
// (member names are ours)
template <typename T>
class Vector3
{
public:
    // the point at t between a (0) and b (1)
    template <typename T1>
    void setLerp(const Vector3& a, const Vector3& b, T1 t)
    {
        x = a.x + (b.x - a.x) * t;
        y = a.y + (b.y - a.y) * t;
        z = a.z + (b.z - a.z) * t;
    }

    T x;
    T y;
    T z;
};
} // namespace sead
