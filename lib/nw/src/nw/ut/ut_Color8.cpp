#include "nw/ut/ut_Color8.h"

namespace nw {
namespace ut {
// 0x007377D0 | nintendogs:bytes [tier A]
u32 nw::ut::Color8::ToU32() const
{
    return (static_cast<u32>(r) << 24) | (static_cast<u32>(g) << 16) | (static_cast<u32>(b) << 8) | a;
}

} // namespace ut
} // namespace nw
