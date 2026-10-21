#pragma once

#include "decomp.h"

namespace nw {
namespace ut {
// A rectangle (the class name is from the reference symbols, member names are ours).
struct Rect
{
    // (inline)
    Rect() {}
    Rect(f32 l, f32 t, f32 r, f32 b) : left(l), top(t), right(r), bottom(b) {}

    f32 GetWidth() const { return right - left; }
    f32 GetHeight() const { return bottom - top; }
    // left <= right and top <= bottom
    void Normalize()
    {
        const f32 l = left;
        const f32 t = top;
        const f32 r = right;
        const f32 b = bottom;
        left = (r - l >= 0.0f) ? l : r;
        right = (r - l >= 0.0f) ? r : l;
        top = (b - t >= 0.0f) ? t : b;
        bottom = (b - t >= 0.0f) ? b : t;
    }

    f32 left;   // 0x0
    f32 top;    // 0x4
    f32 right;  // 0x8
    f32 bottom; // 0xC
};
ASSERT_SIZE(Rect, 0x10);
} // namespace ut
} // namespace nw
