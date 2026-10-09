#include "nn/gr/CTR/gr_RenderState_Blend.h"
#include "nn/gr/CTR/detail/gr_Command.h"

namespace nn {
namespace gr {
namespace CTR {
using detail::CommandHeader;
// 0x0012DF08 | fefates:bytes [tier B]
u32* nn::gr::CTR::RenderState::Blend::MakeCommand(u32* command, bool isUpdateFBAccess) const
{
    // color operation: blend mode
    *command++ = 0xE40100;
    if (m_IsEnable) {
        *command++ = CommandHeader(0x100, 0x3);
        *command++ = m_EquationRgb | (m_EquationAlpha << 8) | (m_SrcRgb << 16) | (m_DstRgb << 20) | (m_SrcAlpha << 24)
            | (m_DstAlpha << 28);
        *command++ = CommandHeader(0x101);
        // logic op: copy
        *command++ = 6;
        *command++ = CommandHeader(0x102);
        *command++ = detail::PackColor(m_ColorR, m_ColorG, m_ColorB, m_ColorA);
        *command++ = CommandHeader(0x103);
    } else {
        *command++ = CommandHeader(0x100, 0x3);
        // source * 1 + destination * 0
        *command++ = 0x01010000;
        *command++ = CommandHeader(0x101);
    }
    if (isUpdateFBAccess) {
        return m_RenderState->m_FBAccess.MakeCommand(command, true);
    }
    return command;
}

} // namespace CTR
} // namespace gr
} // namespace nn
