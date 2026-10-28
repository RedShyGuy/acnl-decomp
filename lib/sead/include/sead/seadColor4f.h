#pragma once

#include "decomp.h"

namespace sead {
// a color of four floats in [0, 1] (member names are ours)
class Color4f
{
public:
    // clamps the components to [0, 1]
    void adjustOverflow_(); // 0x0055DFF8 | nintendogs:bytes [tier A]
    // the color at t (clamped to [0, 1]) between a and b
    void setLerp(const Color4f& a, const Color4f& b, f32 t); // 0x0055E11C | nintendogs:bytes [tier A]
    static Color4f lerp(const Color4f& a, const Color4f& b, f32 t); // 0x0055E0A8 (name is ours)

    f32 r;
    f32 g;
    f32 b;
    f32 a;
};
ASSERT_SIZE(Color4f, 0x10);
} // namespace sead
