#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_RenderState.h"
#include "nn/gr/CTR/gr_Scissor.h"
#include "nn/gr/CTR/gr_Viewport.h"
#include "sead/seadGraphics.h"

namespace sead {
// RTTI N4sead8GraphicsCtrE
// vtable 0x00904FAC (vptr 0x00904FB4), offset_to_top 0, 28 entries
//
// The graphics state of the CTR: each slot changes the copy of the render state and writes its
// commands into the current command buffer. Names are ours (setScissorImpl after the reference
// symbols).
class GraphicsCtr : public ::sead::Graphics
{
public:
    // a viewport and a scissor of 240 x 320 (the frame buffer of a screen)
    GraphicsCtr(); // 0x0011E8A0
    virtual ~GraphicsCtr(); // 0x0054045C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00540440 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x0C(); // 0x0053FE1C slot 0x0C
    virtual void setViewportImpl(f32 x, f32 y, f32 width, f32 height); // 0x0053FEAC slot 0x10
    virtual void setScissorImpl(f32 x, f32 y, f32 width, f32 height); // 0x0053FE20 slot 0x14 | mk7dlp:bytes-fuzzy
    virtual void setDepthTestEnableImpl(bool enable); // 0x0054026C slot 0x18
    virtual void setDepthWriteEnableImpl(bool enable); // 0x00540308 slot 0x1C
    virtual void setDepthFuncImpl(u8 func); // 0x0053FF7C slot 0x20
    virtual bool vf_0x24(); // 0x00540434 slot 0x24
    // -16: no culling (-15 leaves it), 0 and 1: the face that is culled
    virtual void setCullingModeImpl(s32 mode); // 0x0054006C slot 0x28
    virtual void setBlendEnableImpl(bool enable); // 0x0053FFCC slot 0x2C
    virtual void vf_0x30(); // 0x00540220 slot 0x30
    virtual void setBlendFactorImpl(u8 srcRgb, u8 dstRgb, u8 srcAlpha, u8 dstAlpha); // 0x00540010 slot 0x34
    virtual void vf_0x38(); // 0x00540224 slot 0x38
    virtual void setBlendEquationImpl(u8 rgb, u8 alpha); // 0x00540180 slot 0x3C
    virtual void vf_0x40(); // 0x00540304 slot 0x40
    virtual void setBlendConstantColorImpl(const Color4f& color); // 0x005403A0 slot 0x44
    virtual void waitForVBlankImpl(); // 0x0053FFC0 slot 0x48
    virtual void setColorMaskImpl(bool r, bool g, bool b, bool a); // 0x0053FEFC slot 0x4C
    virtual void vf_0x50(); // 0x005400DC slot 0x50
    virtual void setAlphaTestEnableImpl(bool enable); // 0x00540228 slot 0x54
    // ref 0..1
    virtual void setAlphaTestFuncImpl(u8 func, f32 ref); // 0x005400E0 slot 0x58
    virtual void setStencilTestEnableImpl(bool enable); // 0x0054034C slot 0x5C
    virtual void setStencilTestFuncImpl(u8 func, s32 ref, u32 mask); // 0x005402B0 slot 0x60
    virtual void setStencilTestOpImpl(u8 fail, u8 zfail, u8 zpass); // 0x005401CC slot 0x64
    virtual void vf_0x68(); // 0x005400D8 slot 0x68
    virtual void vf_0x6C(); // 0x0054043C slot 0x6C

    // the size of the buffer that the scissor is set for (name is ours)
    static s32 s_ScissorBufferSize[2];

    u8 field_0x18[0x1C];                     // 0x18 (not touched here)
    nn::gr::CTR::RenderState mRenderState;    // 0x34
    nn::gr::CTR::Viewport mViewport;          // 0xB4
    nn::gr::CTR::Scissor mScissor;            // 0xC4
};
ASSERT_SIZE(GraphicsCtr, 0xE0);
} // namespace sead
