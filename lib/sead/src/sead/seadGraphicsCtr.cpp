#include "sead/seadGraphicsCtr.h"
#include "sead/seadColor4f.h"
#include "sead/seadCommandBufferCtr.h"

namespace sead {
// 0x00AE6520
s32 GraphicsCtr::s_ScissorBufferSize[2];

// 0x0011E8A0 (name is ours)
sead::GraphicsCtr::GraphicsCtr() : Graphics(), mRenderState()
{
    mViewport.m_X = 0;
    mViewport.m_Y = 0;
    mViewport.m_Width = 240;
    mViewport.m_Height = 320;
    mScissor.m_IsEnable = true;
    mScissor.m_X = 0;
    mScissor.m_Y = 0;
    mScissor.m_Width = 240;
    mScissor.m_Height = 320;
    mScissor.m_BufferWidth = 256;
    mScissor.m_BufferHeight = 320;
}

// 0x0054045C slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x00540440 (deleting dtor)
sead::GraphicsCtr::~GraphicsCtr()
{
    // (only the bases are destroyed)
}

// 0x0053FE1C slot 0x0C
void sead::GraphicsCtr::vf_0x0C()
{
    // empty (in the original too)
}

// 0x0053FEAC slot 0x10 (name is ours)
void sead::GraphicsCtr::setViewportImpl(f32 x, f32 y, f32 width, f32 height)
{
    mViewport.m_X = static_cast<s32>(x);
    mViewport.m_Y = static_cast<s32>(y);
    mViewport.m_Width = static_cast<s32>(width);
    mViewport.m_Height = static_cast<s32>(height);
    u32* command = getCurrentCommand();
    u32* start = command;
    command = mViewport.MakeCommand(command);
    moveCommandBufferPointer(start, command);
}

// 0x0053FE20 slot 0x14 | mk7dlp:bytes-fuzzy
void sead::GraphicsCtr::setScissorImpl(f32 x, f32 y, f32 width, f32 height)
{
    u32* command = getCurrentCommand();
    u32* start = command;
    mScissor.m_X = static_cast<s32>(x);
    mScissor.m_Y = static_cast<s32>(y);
    mScissor.m_Width = static_cast<s32>(width);
    mScissor.m_Height = static_cast<s32>(height);
    mScissor.m_BufferWidth = s_ScissorBufferSize[0];
    mScissor.m_BufferHeight = s_ScissorBufferSize[1];
    command = mScissor.MakeCommand(command);
    moveCommandBufferPointer(start, command);
}

// 0x0054026C slot 0x18 (name is ours)
void sead::GraphicsCtr::setDepthTestEnableImpl(bool enable)
{
    u32* command = getCurrentCommand();
    u32* start = command;
    mRenderState.m_DepthTest.m_IsEnable = enable;
    command = mRenderState.m_DepthTest.MakeCommand(command, true);
    moveCommandBufferPointer(start, command);
}

// 0x00540308 slot 0x1C (name is ours)
void sead::GraphicsCtr::setDepthWriteEnableImpl(bool enable)
{
    u32* command = getCurrentCommand();
    u32* start = command;
    mRenderState.m_DepthTest.m_IsEnableWrite = enable;
    command = mRenderState.m_DepthTest.MakeCommand(command, true);
    moveCommandBufferPointer(start, command);
}

// 0x0053FF7C slot 0x20 (name is ours)
void sead::GraphicsCtr::setDepthFuncImpl(u8 func)
{
    u32* command = getCurrentCommand();
    u32* start = command;
    mRenderState.m_DepthTest.m_Func = func;
    command = mRenderState.m_DepthTest.MakeCommand(command, true);
    moveCommandBufferPointer(start, command);
}

// 0x00540434 slot 0x24
bool sead::GraphicsCtr::vf_0x24()
{
    return false;
}

// 0x0054006C slot 0x28 (name is ours)
void sead::GraphicsCtr::setCullingModeImpl(s32 mode)
{
    u32* command = getCurrentCommand();
    u32* start = command;
    switch (mode) {
    case -16:
        mRenderState.m_Culling.m_IsEnable = false;
        break;
    case -15:
        break;
    case 0:
    case 1:
        mRenderState.m_Culling.m_IsEnable = true;
        mRenderState.m_Culling.m_CullFace = mode;
        break;
    default:
        break;
    }
    command = mRenderState.m_Culling.MakeCommand(command, true);
    moveCommandBufferPointer(start, command);
}

// 0x0053FFCC slot 0x2C (name is ours)
void sead::GraphicsCtr::setBlendEnableImpl(bool enable)
{
    u32* command = getCurrentCommand();
    u32* start = command;
    mRenderState.m_Blend.m_IsEnable = enable;
    command = mRenderState.m_Blend.MakeCommand(command, true);
    moveCommandBufferPointer(start, command);
}

// 0x00540220 slot 0x30
void sead::GraphicsCtr::vf_0x30()
{
    // empty (in the original too)
}

// 0x00540010 slot 0x34 (name is ours)
void sead::GraphicsCtr::setBlendFactorImpl(u8 srcRgb, u8 dstRgb, u8 srcAlpha, u8 dstAlpha)
{
    u32* command = getCurrentCommand();
    u32* start = command;
    mRenderState.m_Blend.m_SrcRgb = srcRgb;
    mRenderState.m_Blend.m_DstRgb = dstRgb;
    mRenderState.m_Blend.m_SrcAlpha = srcAlpha;
    mRenderState.m_Blend.m_DstAlpha = dstAlpha;
    command = mRenderState.m_Blend.MakeCommand(command, true);
    moveCommandBufferPointer(start, command);
}

// 0x00540224 slot 0x38
void sead::GraphicsCtr::vf_0x38()
{
    // empty (in the original too)
}

// 0x00540180 slot 0x3C (name is ours)
void sead::GraphicsCtr::setBlendEquationImpl(u8 rgb, u8 alpha)
{
    u32* command = getCurrentCommand();
    u32* start = command;
    mRenderState.m_Blend.m_EquationRgb = rgb;
    mRenderState.m_Blend.m_EquationAlpha = alpha;
    command = mRenderState.m_Blend.MakeCommand(command, true);
    moveCommandBufferPointer(start, command);
}

// 0x00540304 slot 0x40
void sead::GraphicsCtr::vf_0x40()
{
    // empty (in the original too)
}

// 0x005403A0 slot 0x44 (name is ours)
void sead::GraphicsCtr::setBlendConstantColorImpl(const Color4f& color)
{
    u32* command = getCurrentCommand();
    u32* start = command;
    Color4u8 color8;
    color8.setColor4f(color);
    mRenderState.m_Blend.m_ColorR = color8.getR();
    mRenderState.m_Blend.m_ColorG = color8.getG();
    mRenderState.m_Blend.m_ColorB = color8.getB();
    mRenderState.m_Blend.m_ColorA = color8.getA();
    command = mRenderState.m_Blend.MakeCommand(command, true);
    moveCommandBufferPointer(start, command);
}

// 0x0053FFC0 slot 0x48 (name is ours)
void sead::GraphicsCtr::waitForVBlankImpl()
{
    // (both displays)
    nngxWaitVSync(0x402);
}

// 0x0053FEFC slot 0x4C (name is ours)
void sead::GraphicsCtr::setColorMaskImpl(bool r, bool g, bool b, bool a)
{
    u32* command = getCurrentCommand();
    u32* start = command;
    mRenderState.m_ColorMask = r | (g ? 2 : 0) | (b ? 4 : 0) | (a ? 8 : 0);
    command = mRenderState.m_FBAccess.MakeCommand(command, true);
    moveCommandBufferPointer(start, command);
}

// 0x005400DC slot 0x50
void sead::GraphicsCtr::vf_0x50()
{
    // empty (in the original too)
}

// 0x00540228 slot 0x54 (name is ours)
void sead::GraphicsCtr::setAlphaTestEnableImpl(bool enable)
{
    u32* command = getCurrentCommand();
    u32* start = command;
    mRenderState.m_AlphaTest.m_IsEnable = enable;
    command = mRenderState.m_AlphaTest.MakeCommand(command, true);
    moveCommandBufferPointer(start, command);
}

// 0x005400E0 slot 0x58 (name is ours)
void sead::GraphicsCtr::setAlphaTestFuncImpl(u8 func, f32 ref)
{
    u32* command = getCurrentCommand();
    u32* start = command;
    mRenderState.m_AlphaTest.m_Func = func;
    f32 value;
    if (ref < 0.0f) {
        value = 0.0f;
    } else if (ref > 1.0f) {
        value = 1.0f;
    } else {
        value = ref;
    }
    mRenderState.m_AlphaTest.m_RefValue = static_cast<u32>(value * 255.0f + 0.5f);
    command = mRenderState.m_AlphaTest.MakeCommand(command, true);
    moveCommandBufferPointer(start, command);
}

// 0x0054034C slot 0x5C (name is ours)
void sead::GraphicsCtr::setStencilTestEnableImpl(bool enable)
{
    u32* command = getCurrentCommand();
    u32* start = command;
    mRenderState.m_StencilTest.m_IsEnable = enable;
    mRenderState.m_StencilTest.m_WriteMask = enable ? 0xFF : 0;
    command = mRenderState.m_StencilTest.MakeCommand(command, true);
    moveCommandBufferPointer(start, command);
}

// 0x005402B0 slot 0x60 (name is ours)
void sead::GraphicsCtr::setStencilTestFuncImpl(u8 func, s32 ref, u32 mask)
{
    u32* command = getCurrentCommand();
    u32* start = command;
    mRenderState.m_StencilTest.m_Func = func;
    mRenderState.m_StencilTest.m_RefValue = ref;
    mRenderState.m_StencilTest.m_Mask = mask;
    command = mRenderState.m_StencilTest.MakeCommand(command, true);
    moveCommandBufferPointer(start, command);
}

// 0x005401CC slot 0x64 (name is ours)
void sead::GraphicsCtr::setStencilTestOpImpl(u8 fail, u8 zfail, u8 zpass)
{
    u32* command = getCurrentCommand();
    u32* start = command;
    mRenderState.m_StencilTest.m_OpFail = fail;
    mRenderState.m_StencilTest.m_OpZFail = zfail;
    mRenderState.m_StencilTest.m_OpZPass = zpass;
    command = mRenderState.m_StencilTest.MakeCommand(command, true);
    moveCommandBufferPointer(start, command);
}

// 0x005400D8 slot 0x68
void sead::GraphicsCtr::vf_0x68()
{
    // empty (in the original too)
}

// 0x0054043C slot 0x6C
void sead::GraphicsCtr::vf_0x6C()
{
    // empty (in the original too)
}
} // namespace sead
