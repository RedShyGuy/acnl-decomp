#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_RenderState.h"
#include "sead/seadColor4f.h"

namespace sead {
// RTTI N4sead15GraphicsContextE @ 0x008D1894
// vtable 0x00905AEC (vptr 0x00905AF4), offset_to_top 0, 2 entries
//
// A set of render states that apply() writes into the command buffer: the values of the sead API
// (depth test, culling, blending, alpha test, color mask, stencil test; values as for the
// GraphicsCtr slots) and the render state of nn::gr. All names are ours.
class GraphicsContext
{
public:
    // depth test and write on, blending on, color mask all, alpha test and stencil test off
    GraphicsContext(); // 0x0012091C
    virtual ~GraphicsContext(); // 0x00545D38 slot 0x00
    // 0x00545D28 slot 0x04 (deleting dtor)

    void apply() const; // 0x00121664 | nintendogs:bytes [tier A]

    bool mDepthTestEnable;      // 0x04
    bool mDepthWriteEnable;     // 0x05
    s32 mDepthFunc;             // 0x08
    s32 mCullingMode;           // 0x0C
    bool mBlendEnable;          // 0x10
    s32 mBlendFactorSrcRgb;     // 0x14
    s32 mBlendFactorSrcAlpha;   // 0x18
    s32 mBlendFactorDstRgb;     // 0x1C
    s32 mBlendFactorDstAlpha;   // 0x20
    s32 mBlendEquationRgb;      // 0x24
    s32 mBlendEquationAlpha;    // 0x28
    Color4f mBlendConstantColor; // 0x2C
    bool mAlphaTestEnable;      // 0x3C
    s32 mAlphaTestFunc;         // 0x40
    f32 mAlphaTestRef;          // 0x44
    bool mColorMaskR;           // 0x48
    bool mColorMaskG;           // 0x49
    bool mColorMaskB;           // 0x4A
    bool mColorMaskA;           // 0x4B
    bool mStencilTestEnable;    // 0x4C
    s32 mStencilTestFunc;       // 0x50
    s32 mStencilTestRef;        // 0x54
    u32 mStencilTestMask;       // 0x58
    s32 mStencilOpFail;         // 0x5C
    s32 mStencilOpZFail;        // 0x60
    s32 mStencilOpZPass;        // 0x64
    nn::gr::CTR::RenderState mRenderState; // 0x68
    u32 field_0xE8;             // 0xE8 (not touched here)
};
ASSERT_SIZE(GraphicsContext, 0xEC);
} // namespace sead
