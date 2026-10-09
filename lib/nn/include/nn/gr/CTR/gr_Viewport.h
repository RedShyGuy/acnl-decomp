#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
// member names are ours
class Viewport
{
public:
    u32* MakeCommand(u32* command) const; // 0x00728B2C | nintendogs:bytes [tier B]

    s32 m_X;        // 0x00
    s32 m_Y;        // 0x04
    u32 m_Width;    // 0x08
    u32 m_Height;   // 0x0C
};
ASSERT_SIZE(Viewport, 0x10);
} // namespace CTR
} // namespace gr
} // namespace nn
