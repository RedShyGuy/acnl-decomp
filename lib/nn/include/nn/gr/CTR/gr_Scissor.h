#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
// member names are ours
class Scissor
{
public:
    u32* MakeCommand(u32* command) const; // 0x00728118 | nintendogs:bytes [tier B]

    bool m_IsEnable;    // 0x00
    s32 m_X;            // 0x04
    s32 m_Y;            // 0x08
    u32 m_Width;        // 0x0C
    u32 m_Height;       // 0x10
    s32 m_BufferWidth;  // 0x14
    s32 m_BufferHeight; // 0x18
};
ASSERT_SIZE(Scissor, 0x1C);
} // namespace CTR
} // namespace gr
} // namespace nn
