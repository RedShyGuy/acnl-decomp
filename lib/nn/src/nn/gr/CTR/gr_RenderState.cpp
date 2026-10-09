#include "nn/gr/CTR/gr_RenderState.h"
#include "nn/gr/CTR/CTR_Api.h"
#include "nn/gr/CTR/detail/gr_Command.h"

namespace nn {
namespace gr {
namespace CTR {
using detail::CommandHeader;
namespace {
inline u32* MakeFlushCommand(u32* command)
{
    *command++ = 1;
    *command++ = CommandHeader(0x111);
    *command++ = 1;
    *command++ = CommandHeader(0x110);
    return command;
}
} // namespace

// 0x00123C08 | fefates:bytes [tier B]
nn::gr::CTR::RenderState::RenderState()
    : m_Blend(this), m_LogicOp(this), m_ShadowMap(this), m_AlphaTest(this), m_StencilTest(this), m_ColorMask(0xF),
      m_DepthTest(this), m_Culling(this), m_FBAccess(this)
{
}

inline u32* nn::gr::CTR::RenderState::LogicOp::MakeCommand(u32* command) const
{
    if (m_IsEnable) {
        // color operation: logic op mode
        *command++ = 0xE40000;
        *command++ = CommandHeader(0x100, 0x3);
        *command++ = 0x01010000;
        *command++ = CommandHeader(0x101);
        *command++ = m_Operation;
        *command++ = CommandHeader(0x102);
    }
    return command;
}

// 0x00727074 | fefates:bytes [tier B]
u32* nn::gr::CTR::RenderState::Culling::MakeCommand(u32* command, bool isUpdateFBAccess) const
{
    // 0 none, 1 and 2 cull one or the other winding
    u32 mode = 0;
    if (m_IsEnable) {
        if ((m_FrontFace == 0 && m_CullFace == 0) || (m_FrontFace == 1 && m_CullFace == 1)) {
            mode = 2;
        } else {
            mode = 1;
        }
    }
    *command++ = mode;
    *command++ = CommandHeader(0x40);
    if (isUpdateFBAccess) {
        return m_RenderState->m_FBAccess.MakeCommand(command, true);
    }
    return command;
}

// 0x007270F0 | fefates:bytes [tier B]
u32* nn::gr::CTR::RenderState::AlphaTest::MakeCommand(u32* command, bool isUpdateFBAccess) const
{
    *command++ = (m_IsEnable ? 1 : 0) | (m_Func << 4) | (m_RefValue << 8);
    *command++ = CommandHeader(0x104, 0x3);
    if (isUpdateFBAccess) {
        return m_RenderState->m_FBAccess.MakeCommand(command, true);
    }
    return command;
}

// 0x00726FE8 | fefates:bytes [tier B]
u32* nn::gr::CTR::RenderState::StencilTest::MakeCommand(u32* command, bool isUpdateFBAccess) const
{
    *command++ = (m_IsEnable ? 1 : 0) | (m_Func << 4) | (m_WriteMask << 8) | (m_RefValue << 16) | (m_Mask << 24);
    *command++ = CommandHeader(0x105);
    *command++ = m_OpFail | (m_OpZFail << 4) | (m_OpZPass << 8);
    *command++ = CommandHeader(0x106);
    if (isUpdateFBAccess) {
        return m_RenderState->m_FBAccess.MakeCommand(command, true);
    }
    return command;
}

// 0x00727148 | fefates:bytes [tier B]
u32* nn::gr::CTR::RenderState::DepthTest::MakeCommand(u32* command, bool isUpdateFBAccess) const
{
    const u8 colorMask = m_RenderState->m_ColorMask;
    *command++ = (m_IsEnable ? 1 : 0) | (m_Func << 4) | ((colorMask & 1) ? 0x100 : 0) | ((colorMask & 2) ? 0x200 : 0)
        | ((colorMask & 4) ? 0x400 : 0) | ((colorMask & 8) ? 0x800 : 0) | (m_IsEnableWrite ? 0x1000 : 0);
    *command++ = CommandHeader(0x107);
    if (isUpdateFBAccess) {
        return m_RenderState->m_FBAccess.MakeCommand(command, true);
    }
    return command;
}

// 0x001269F0 | nintendogs:callgraph [confirmed by mk7dlp] [tier A]
u32* nn::gr::CTR::RenderState::MakeCommand(u32* command, bool isFlush) const
{
    if (isFlush) {
        command = MakeFlushCommand(command);
    }
    command = m_Culling.MakeCommand(command, false);
    command = m_Blend.MakeCommand(command, false);
    command = m_LogicOp.MakeCommand(command);
    command = m_ShadowMap.MakeCommand(command, false, true);
    command = m_AlphaTest.MakeCommand(command, false);
    command = m_StencilTest.MakeCommand(command, false);
    command = m_DepthTest.MakeCommand(command, false);
    command = m_WBuffer.MakeCommand(command);
    return m_FBAccess.MakeCommand(command, false);
}

// 0x0012DFC8 (name is ours)
u32* nn::gr::CTR::RenderState::WBuffer::MakeCommand(u32* command) const
{
    if (m_WScale == 0.0f) {
        *command++ = 1;
        *command++ = CommandHeader(0x6D);
        *command++ = Float32ToFloat24(m_DepthRangeNear - m_DepthRangeFar);
        *command++ = CommandHeader(0x4D);
        f32 offset = m_DepthRangeNear;
        if (m_IsEnablePolygonOffset) {
            offset -= (m_DepthRangeNear - m_DepthRangeFar) * 128.0f * m_PolygonOffsetUnit
                / static_cast<f32>((1 << m_DepthBufferBits) - 1);
        }
        *command++ = Float32ToFloat24(offset);
        *command++ = CommandHeader(0x4E);
    } else {
        *command++ = 0;
        *command++ = CommandHeader(0x6D);
        *command++ = Float32ToFloat24(-m_WScale);
        *command++ = CommandHeader(0x4D);
        // the offset goes through a float variable (original behavior)
        f32 offset = 0.0f;
        if (m_IsEnablePolygonOffset) {
            if (m_DepthBufferBits == 24) {
                offset = Float32ToFloat24(m_PolygonOffsetUnit * 128.0f / 16777215.0f);
            } else {
                offset = Float32ToFloat24(m_PolygonOffsetUnit / static_cast<f32>((1 << m_DepthBufferBits) - 1));
            }
        }
        *command++ = static_cast<u32>(offset);
        *command++ = CommandHeader(0x4E);
    }
    return command;
}

// 0x00349984 (name is ours)
u32* nn::gr::CTR::RenderState::MakeDisableCommand(u32* command, bool isFlush)
{
    *command++ = 0;
    *command++ = CommandHeader(0x40, 0x1);
    *command++ = 0xE40100;
    *command++ = CommandHeader(0x100, 0x3);
    *command++ = 0x01010000;
    *command++ = CommandHeader(0x101);
    *command++ = 0;
    *command++ = CommandHeader(0x104, 0x1);
    *command++ = 0;
    *command++ = CommandHeader(0x105, 0x1);
    // depth test off, function 4, all colors written
    *command++ = 0xF40;
    *command++ = CommandHeader(0x107);
    if (isFlush) {
        command = MakeFlushCommand(command);
    }
    *command++ = 0xF;
    *command++ = CommandHeader(0x112, 0x1, 3, true);
    *command++ = 0xF;
    *command++ = 0;
    *command++ = 0;
    *command++ = 0;
    return command;
}

} // namespace CTR
} // namespace gr
} // namespace nn
