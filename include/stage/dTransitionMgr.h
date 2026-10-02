#pragma once

#include "decomp.h"

namespace stage {
class TransitionMgr
{
public:
    void Transition(stage::Name, unsigned char, unsigned char, unsigned char, unsigned char); // 0x005B5864 | libgarden [tier A]
    void SetReturnParam(); // 0x005B64AC | libgarden [tier A]
};
} // namespace stage
