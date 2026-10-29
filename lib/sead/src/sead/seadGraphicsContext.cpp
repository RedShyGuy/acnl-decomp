#include "sead/seadGraphicsContext.h"
#include "sead/seadCommandBufferCtr.h"

namespace sead {
// 0x0012091C (name is ours)
sead::GraphicsContext::GraphicsContext()
    : mDepthTestEnable(true), mDepthWriteEnable(true), mDepthFunc(5), mCullingMode(1), mBlendEnable(true),
      mBlendFactorSrcRgb(6), mBlendFactorSrcAlpha(6), mBlendFactorDstRgb(7), mBlendFactorDstAlpha(7),
      mBlendEquationRgb(0), mBlendEquationAlpha(0), mAlphaTestEnable(false), mAlphaTestFunc(6), mAlphaTestRef(0.0f),
      mColorMaskR(true), mColorMaskG(true), mColorMaskB(true), mColorMaskA(true), mStencilTestEnable(false),
      mStencilTestFunc(0), mStencilTestRef(0), mStencilTestMask(0xFFFFFFFF), mStencilOpFail(0), mStencilOpZFail(0),
      mStencilOpZPass(0), mRenderState()
{
    mBlendConstantColor.r = 1.0f;
    mBlendConstantColor.g = 1.0f;
    mBlendConstantColor.b = 1.0f;
    mBlendConstantColor.a = 1.0f;
    mRenderState.m_DepthTest.m_Func = 7;
    mRenderState.m_Blend.m_IsEnable = true;
    mRenderState.m_AlphaTest.m_Func = 6;
    mRenderState.m_ColorMask = 15;
    mRenderState.m_StencilTest.m_Func = 0;
    mRenderState.m_StencilTest.m_Mask = 0xFFFFFFFF;
}

// 0x00545D38 slot 0x00
// 0x00545D28 (deleting dtor)
sead::GraphicsContext::~GraphicsContext()
{
    // empty (in the original too)
}

// 0x00121664 | nintendogs:bytes [tier A]
void sead::GraphicsContext::apply() const
{
    u32* command = getCurrentCommand();
    u32* start = command;
    command = mRenderState.MakeCommand(command, true);
    moveCommandBufferPointer(start, command);
}
} // namespace sead
