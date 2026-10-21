#pragma once

#include "decomp.h"

namespace nw {
namespace ut {
// An RGBA color with 8 bits per channel. Member names are ours.
class Color8
{
public:
    // white (inline; the copy out of line constructs arrays with __aeabi_vec_ctor)
    DECOMP_EMIT_COPY Color8() : r(0xFF), g(0xFF), b(0xFF), a(0xFF) {} // 0x0048BA60 (name is ours)
    // (inline)
    Color8(u8 red, u8 green, u8 blue, u8 alpha) : r(red), g(green), b(blue), a(alpha) {}

    // the color as 0xRRGGBBAA
    u32 ToU32() const; // 0x007377D0 | nintendogs:bytes [tier A]

    u8 r; // 0x0
    u8 g; // 0x1
    u8 b; // 0x2
    u8 a; // 0x3
};
ASSERT_SIZE(Color8, 0x4);
} // namespace ut
} // namespace nw
