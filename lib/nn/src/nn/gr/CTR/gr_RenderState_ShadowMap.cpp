#include "nn/gr/CTR/gr_RenderState_ShadowMap.h"
#include "nn/gr/CTR/detail/gr_Command.h"

namespace nn {
namespace gr {
namespace CTR {
using detail::CommandHeader;
// 0x0012E1E8 | fefates:bytes [tier B]
u32* nn::gr::CTR::RenderState::ShadowMap::MakeCommand(u32* command, bool isUpdateFBAccess, bool isResetTextureConfig) const
{
    if (m_IsEnable) {
        // color operation: shadow mode
        *command++ = 0xE40103;
        *command++ = CommandHeader(0x100, 0x1);
        if (isResetTextureConfig) {
            *command++ = 0;
            *command++ = CommandHeader(0x80, 0x0, 2);
            *command++ = 0;
            *command++ = 0;
        }
        *command++ = ((detail::Float32ToUnsignedFix24(m_ZBias) >> 1) << 1) | (m_IsPerspective ? 0 : 1);
        *command++ = CommandHeader(0x8B);
        *command++ = detail::Float32ToFloat16(m_PenumbraScale + m_PenumbraBias) | (detail::Float32ToFloat16(-m_PenumbraScale) << 16);
        *command++ = CommandHeader(0x130);
    }
    if (isUpdateFBAccess) {
        return m_RenderState->m_FBAccess.MakeCommand(command, true);
    }
    return command;
}

} // namespace CTR
} // namespace gr
} // namespace nn
