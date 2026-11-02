#pragma once

#include "decomp.h"

namespace math {
class Angle16
{
public:
    void Atan2(float, float); // 0x005667A8 | libgarden [tier A]

    u16 value; // 0x0 (name after libgarden)
};
} // namespace math
