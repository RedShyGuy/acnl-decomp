#pragma once

#include "decomp.h"

namespace nn {
namespace math {
class MTX34
{
public:
    void Identity(); // 0x0047E49C | nintendogs:callgraph [tier A]

    f32 m[3][4];    // row-major, column 3 is the translation
};
ASSERT_SIZE(MTX34, 0x30);
} // namespace math
} // namespace nn
