#pragma once

#include "decomp.h"

namespace sead {
// a color of four floats in [0, 1] (member names are ours)
class Color4f
{
public:
    // black (from the two constants below, like the original)
    Color4f() : r(cElementMin), g(cElementMin), b(cElementMin), a(cElementMax) {}
    constexpr Color4f(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}

    // clamps the components to [0, 1]
    void adjustOverflow_(); // 0x0055DFF8 | nintendogs:bytes [tier A]
    // the color at t (clamped to [0, 1]) between a and b
    void setLerp(const Color4f& a, const Color4f& b, f32 t); // 0x0055E11C | nintendogs:bytes [tier A]
    static Color4f lerp(const Color4f& a, const Color4f& b, f32 t); // 0x0055E0A8 (name is ours)

    // (the names are ours)
    static const Color4f white;
    static const f32 cElementMax;
    static const f32 cElementMin;

    f32 r;
    f32 g;
    f32 b;
    f32 a;
};
ASSERT_SIZE(Color4f, 0x10);

// a color of four bytes, read as the word 0xRRGGBBAA (the class and member names are ours)
class Color4u8
{
public:
    Color4u8() : a(255), b(0), g(0), r(0) {}

    // the components of color (clamped to [0, 1]) times 255
    void setColor4f(Color4f color); // 0x00560CA0

    u8 getR() const { return r; }
    u8 getG() const { return g; }
    u8 getB() const { return b; }
    u8 getA() const { return a; }

    u8 a; // 0x0
    u8 b; // 0x1
    u8 g; // 0x2
    u8 r; // 0x3
};
ASSERT_SIZE(Color4u8, 0x4);
} // namespace sead
