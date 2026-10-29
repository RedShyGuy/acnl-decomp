#pragma once

#include "decomp.h"

namespace sead {
// A two-dimensional vector (member names are ours; the constants are defined in seadMathConstants.cpp).
template <typename T>
class Vector2
{
public:
    Vector2() = default;
    constexpr Vector2(T x_, T y_) : x(x_), y(y_) {}

    static const Vector2 zero;
    static const Vector2 ones;

    T x;
    T y;
};
} // namespace sead
