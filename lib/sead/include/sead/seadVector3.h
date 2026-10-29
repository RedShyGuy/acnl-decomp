#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"

namespace sead {
// Instantiations found in the binary:
//   sead::Vector3<int>  typeinfo 0x008D2164
// (member names are ours)
template <typename T>
class Vector3
{
public:
    Vector3() = default;
    constexpr Vector3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

    // the point at t between a (0) and b (1)
    template <typename T1>
    void setLerp(const Vector3& a, const Vector3& b, T1 t)
    {
        x = a.x + (b.x - a.x) * t;
        y = a.y + (b.y - a.y) * t;
        z = a.z + (b.z - a.z) * t;
    }

    static const Vector3 zero;
    static const Vector3 ones;

    T x;
    T y;
    T z;
};

// sead::Vector3<f32> is laid out like nn::math::VEC3, the type of the CalcCtr functions (names are
// ours)
inline nn::math::VEC3& toVEC3(Vector3<f32>& v)
{
    return reinterpret_cast<nn::math::VEC3&>(v);
}

inline const nn::math::VEC3& toVEC3(const Vector3<f32>& v)
{
    return reinterpret_cast<const nn::math::VEC3&>(v);
}
} // namespace sead
