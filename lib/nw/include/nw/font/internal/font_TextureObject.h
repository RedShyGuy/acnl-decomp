#pragma once

#include "decomp.h"

namespace nw {
namespace font {
namespace internal {
class TextureObject
{
public:
    void Set(unsigned, const nw::font::Font*, const void*, unsigned short, unsigned short, unsigned short); // 0x00141B5C | nintendogs:bytes [tier A]
};
} // namespace internal
} // namespace font
} // namespace nw
