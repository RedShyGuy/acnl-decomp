#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// The value that a completion callback gets back (a number or a pointer). The names are ours.
class UserContext
{
public:
    union
    {
        u32 m_Value;
        void* m_pPointer;
    };
};
ASSERT_SIZE(UserContext, 0x4);
} // namespace nex
} // namespace nn
