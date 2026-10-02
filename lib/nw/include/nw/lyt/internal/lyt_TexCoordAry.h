#pragma once

#include "decomp.h"

namespace nw {
namespace lyt {
namespace internal {
class TexCoordAry
{
public:
    void Copy(const void*, unsigned char); // 0x004BE8E8 | nintendogs:bytes [tier A]
    void Free(); // 0x004BE988 | nintendogs:bytes [tier A]
    void Reserve(unsigned char); // 0x004BE9B4 | libgarden [tier A]
    void SetSize(unsigned char); // 0x004BEA34 | nintendogs:bytes-fuzzy [tier A]
    void SetCoord(unsigned, const nn::math::VEC2*); // 0x004BEB80 | mk7dlp:bytes [tier B]
    TexCoordAry(); // 0x004BEBF8 | nintendogs:bytes [tier A]
};
} // namespace internal
} // namespace lyt
} // namespace nw
