#include "nn/gr/CTR/gr_CommandBufferJumpHelper.h"

namespace nn {
namespace gr {
namespace CTR {
// 0x00349D64 | fefates:bytes [tier B]
void nn::gr::CTR::CommandBufferJumpHelper::FinalizeJump(u32* command)
{
    m_Current = command;
    if (m_JumpSize != NULL) {
        // in units of 8 bytes
        *m_JumpSize = (reinterpret_cast<uptr>(command) - reinterpret_cast<uptr>(m_JumpBase)) >> 3;
    }
    m_Start = m_Current;
    m_JumpSize = NULL;
    m_JumpBase = m_Current;
    m_Unknown10 = 0;
}

// 0x00349DA0 | fefates:bytes [tier B]
nn::gr::CTR::CommandBufferJumpHelper::CommandBufferJumpHelper(u32* buffer)
    : m_Current(buffer), m_Start(buffer), m_JumpBase(buffer), m_JumpSize(NULL), m_Unknown10(0)
{
}

} // namespace CTR
} // namespace gr
} // namespace nn
