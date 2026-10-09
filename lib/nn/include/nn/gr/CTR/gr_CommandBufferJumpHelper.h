#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
// Tracks a command buffer that is split into pieces linked by channel jumps; FinalizeJump writes
// the size of the finished piece into the jump command that leads to it (member names are ours).
class CommandBufferJumpHelper
{
public:
    explicit CommandBufferJumpHelper(u32* buffer); // 0x00349DA0 | fefates:bytes [tier B]
    void FinalizeJump(u32* command); // 0x00349D64 | fefates:bytes [tier B]

private:
    u32* m_Current;     // 0x00
    u32* m_Start;       // 0x04
    u32* m_JumpBase;    // 0x08, start of the piece the pending jump leads to
    u32* m_JumpSize;    // 0x0C, size parameter of the pending jump (NULL if none)
    u32 m_Unknown10;    // 0x10
};
ASSERT_SIZE(CommandBufferJumpHelper, 0x14);
} // namespace CTR
} // namespace gr
} // namespace nn
