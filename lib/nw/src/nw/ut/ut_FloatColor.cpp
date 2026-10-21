#include "nw/ut/ut_FloatColor.h"

namespace nw {
namespace ut {
namespace {
// the channel as a byte: clamped to 0.0 - 1.0 and rounded (inline; name is ours)
inline u8 ToU8(f32 value)
{
    if (value > 1.0f) {
        value = 1.0f;
    } else if (value < 0.0f) {
        value = 0.0f;
    }
    return static_cast<u8>(static_cast<u32>(value * 255.0f + 0.5f));
}
} // namespace

// 0x007375CC | nintendogs:bytes [tier A]
u32 nw::ut::FloatColor::ToPicaU32() const
{
    u8 red = ToU8(r);
    u8 green = ToU8(g);
    u8 blue = ToU8(b);
    u8 alpha = ToU8(a);
    return (static_cast<u32>(alpha) << 24) | (static_cast<u32>(blue) << 16) | (static_cast<u32>(green) << 8) | red;
}

} // namespace ut
} // namespace nw
