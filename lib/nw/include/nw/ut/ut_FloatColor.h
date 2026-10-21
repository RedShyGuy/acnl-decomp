#pragma once

#include "decomp.h"

namespace nw {
namespace ut {
// An RGBA color with a float (0.0 - 1.0) per channel. Member names are ours.
class FloatColor
{
public:
    // black, opaque (inline)
    FloatColor() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}

    // the color for the GPU: 0xAABBGGRR, every channel clamped and rounded
    u32 ToPicaU32() const; // 0x007375CC | nintendogs:bytes [tier A]

    f32 r; // 0x0
    f32 g; // 0x4
    f32 b; // 0x8
    f32 a; // 0xC
};
ASSERT_SIZE(FloatColor, 0x10);
} // namespace ut
} // namespace nw
