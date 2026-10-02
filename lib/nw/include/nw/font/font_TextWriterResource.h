#pragma once

#include "decomp.h"

namespace nw {
namespace font {
class TextWriterResource
{
public:
    void SetViewMtx(nn::math::MTX34 const&); // 0x007ADEC8 | libgarden [tier A]
};
} // namespace font
} // namespace nw
