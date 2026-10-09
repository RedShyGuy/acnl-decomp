#include "nn/gr/CTR/gr_Scissor.h"
#include "nn/gr/CTR/detail/gr_Command.h"

namespace nn {
namespace gr {
namespace CTR {
// 0x00728118 | nintendogs:bytes [tier B]
u32* nn::gr::CTR::Scissor::MakeCommand(u32* command) const
{
    const s32 right = m_X + m_Width - 1;
    const s32 bottom = m_Y + m_Height - 1;
    *command++ = m_IsEnable ? 3 : 0;
    *command++ = detail::CommandHeader(0x65, 0xF, 2, true);

    s32 x = m_X;
    if (x < 0) {
        x = 0;
    } else if (x >= m_BufferWidth) {
        x = m_BufferWidth - 1;
    }
    s32 y = m_Y;
    if (y < 0) {
        y = 0;
    } else if (y >= m_BufferHeight) {
        y = m_BufferHeight - 1;
    }
    *command++ = x | (y << 16);
    // (only the left/top corner is clamped to the buffer)
    *command++ = (right < 0) ? 0 : (right | ((bottom < 0 ? 0 : bottom) << 16));
    return command;
}

} // namespace CTR
} // namespace gr
} // namespace nn
