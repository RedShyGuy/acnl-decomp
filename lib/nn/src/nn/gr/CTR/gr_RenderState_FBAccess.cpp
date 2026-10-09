#include "nn/gr/CTR/gr_RenderState_FBAccess.h"
#include "nn/gr/CTR/detail/gr_Command.h"

namespace nn {
namespace gr {
namespace CTR {
using detail::CommandHeader;
namespace {
// blend factors that read the destination (destination color/alpha and their inverses, and
// source alpha saturate)
inline bool IsReadingDestination(u8 factor)
{
    return factor == 4 || factor == 5 || factor == 8 || factor == 9 || factor == 14;
}
} // namespace

// 0x00134564 | fefates:bytes [tier B]
u32* nn::gr::CTR::RenderState::FBAccess::MakeCommand(u32* command, bool isFlush) const
{
    if (isFlush) {
        *command++ = 1;
        *command++ = CommandHeader(0x111);
        *command++ = 1;
        *command++ = CommandHeader(0x110);
    }

    if (m_RenderState->m_ShadowMap.m_IsEnable) {
        *command++ = 0xF;
        *command++ = CommandHeader(0x112, 0x1);
        *command++ = 0xF;
        *command++ = CommandHeader(0x113, 0x1);
        *command++ = 0;
        *command++ = CommandHeader(0x114, 0x1);
        *command++ = 0;
        *command++ = CommandHeader(0x115, 0x1);
        return command;
    }

    u32 colorRead = 0;
    const RenderState& state = *m_RenderState;
    if (state.m_ColorMask != 0) {
        if (state.m_ColorMask != 0xF) {
            colorRead = 0xF;
        } else if (state.m_Blend.m_IsEnable
                   && (state.m_Blend.m_DstRgb != 0 || state.m_Blend.m_DstAlpha != 0
                       || IsReadingDestination(state.m_Blend.m_SrcRgb) || IsReadingDestination(state.m_Blend.m_SrcAlpha))) {
            colorRead = 0xF;
        } else if (state.m_LogicOp.m_IsEnable) {
            // clear, set, copy and inverted copy do not read the destination
            const u8 operation = state.m_LogicOp.m_Operation;
            if (operation != 0 && operation != 3 && operation != 4 && operation != 5) {
                colorRead = 0xF;
            }
        }
    }
    *command++ = colorRead;
    *command++ = CommandHeader(0x112, 0x1);
    *command++ = (m_RenderState->m_ColorMask != 0) ? 0xF : 0;
    *command++ = CommandHeader(0x113, 0x1);

    u32 depthRead = 0;
    u32 depthWrite = 0;
    const RenderState& current = *m_RenderState;
    if (current.m_DepthTest.m_IsEnable) {
        if (current.m_DepthTest.m_IsEnableWrite) {
            depthRead = 2;
            depthWrite = 2;
        } else if (current.m_ColorMask != 0) {
            depthRead = 2;
        }
    }
    if (current.m_StencilTest.m_IsEnable) {
        if (current.m_StencilTest.m_WriteMask != 0) {
            depthRead |= 1;
            depthWrite |= 1;
        } else if (current.m_ColorMask != 0) {
            depthRead |= 1;
        }
    }
    *command++ = depthRead;
    *command++ = CommandHeader(0x114, 0x1);
    *command++ = depthWrite;
    *command++ = CommandHeader(0x115, 0x1);
    return command;
}

} // namespace CTR
} // namespace gr
} // namespace nn
